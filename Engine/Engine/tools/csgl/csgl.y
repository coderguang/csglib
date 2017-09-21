%{

#include "csgl.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

using namespace std;

void csgInfo(std::string type,std::string s);

std::string toStr(int);


extern FILE *yyin;
extern FILE *yyout;

ofstream ofs;
ofstream ofscpp;

// headfile content
CSGStream headDefStream;
CSGStream endDefStream;
CSGStream headIncludeStream;
CSGStream headfileContentStream;

//cppfile content
CSGStream cppContentStream;

//output info stream
CSGStream outputInfo;


//interface content
CSGStream headInterfaceStream;
CSGStream cppInterfaceStream;



bool globalError=false;

std::vector<std::string> structVec;

%}
// ifndef token
%token TOKEN_IFNDEF TOKEN_IFNDEF_END TOKEN_IFDEF TOKEN_IFDEF_END 
%token <t_string> TOKEN_IFNDEF_FILE TOKEN_IFDEF_FILE TOKEN_ENDIF

//comment token
%token TOKEN_COMMENT_START TOKEN_COMMENT_END TOKEN_COMMENT_EX_START TOKEN_COMMENT_EX_END
%token <t_string> TOKEN_COMMENT_FILE TOKEN_COMMENT_EX_FILE

//include token
%token TOKEN_INCLUDE_START TOKEN_INCLUDE_END TOKEN_INCLUDE_SIGN
%token <t_string> TOKEN_INCLUDE_FILE

//enum
%token <t_string>TOKEN_ENUM_START 
%token <te_list>TOKEN_ENUM_CONTENT;

//struct
%token TOKEN_STRUCT_START TOKEN_STRUCT_END
%token <t_string> TOKEN_STRUCT_NAME
%token <tstruct_list> TOKEN_STRUCT_CONTENT

//id
%token TOKEN_ID_START TOKEN_ID_END
%token <id> TOKEN_ID_NUM

//interface
%token TOKEN_INTERFACE_END TOKEN_INTERFACE_ID_START TOKEN_INTERFACE_ID_NUM TOKEN_INTERFACE_ID_END
%token <t_string> TOKEN_INTERFACE_NAME
%token <tinterface_list> TOKEN_INTERFACE_CONTENT
%token <tmpInterfaceKey.funcName> TOKEN_INTERFACE_FUNCTION_NAME


%%

program : program statement|statement{};

statement : define_line | comment |include | enum |struct |interface{};


interface : TOKEN_INTERFACE_NAME TOKEN_INTERFACE_CONTENT {
	//classNameDef
	std::string interfaceName=$1;
	std::vector<CSGInterface> interfaceList=$2;
	bool isFirstOutParam=true;
	bool isFirstParam=true;

	outputInfo<<"yacc match interface:"<<interfaceName<<"\n";
	//classNameDef
	std::string interfaceClassName=interfaceName;
	std::string srvCBprefix="CSrv_"+interfaceClassName+"_";
	std::string cliCBprefix="CCli_"+interfaceClassName+"_";

	std::string mNamespace="Message";
	std::string proxyNamespace="csg_proxy";

	//======================Server callBack start==============
	CSGStream callBackDef;
	CSGStream cppCallBack;

	callBackDef<<"namespace "<<mNamespace<<"\n"
				<<"{\n";

	for(int i=0;i<interfaceList.size();i++){
		CSGInterface tmp=interfaceList[i];
		std::string functionName=tmp.funcName;

		//class def
		std::string srvCB=srvCBprefix+functionName;
		std::string srvCBPtr=srvCB+"_Ptr";
		std::string cliCB=cliCBprefix+functionName;
		std::string cliCBPtr=cliCB+"_Ptr";

		callBackDef<<"	class "<<srvCB<<":public virtual CRMIObjectCallBackObject\n"
								<<"	{\n"
								<<"	public:\n"
								<<"		virtual void response(";
		cppCallBack<<"void Message::"<<srvCB<<"::response(";

		CSGStream cppCallBackWrite;

		isFirstOutParam=true;
		isFirstParam=true;

		for(int j=0;j<tmp.paramList.size();j++){
			CSGInterfaceParam param=tmp.paramList[j];
			bool isOut=param.isOut;
			std::string type=param.type;
			std::string identify=param.identify;

			if(isOut){
				if(isFirstOutParam){
					callBackDef<<param.type<<" "<<param.identify;
					cppCallBack<<param.type<<" "<<param.identify;
					isFirstOutParam=false;
				}else{
					callBackDef<<","<<param.type<<" "<<param.identify;		
					cppCallBack<<","<<param.type<<" "<<param.identify;		
				}
				cppCallBackWrite<<"	__os->write("<<param.identify<<");\n";
			}
		}

		callBackDef<<");\n"
					<<"	};\n"
					<<"	typedef CSmartPointShare<"<<srvCB<<"> "<<srvCBPtr<<";\n\n";
		cppCallBack<<")\n"
					<<"{\n"
					<<"	CAutoSerializeStream __os(CSerializeStreamPool::instance()->newObject());\n"
					<<"	SRMIReturn rmiReturn;\n"
					<<"	rmiReturn.dispatchStatus = ERMIDispatchResultOk;\n"
					<<"	rmiReturn.messageId = _rmiCall.messageId;\n\n"
					<<"	CRpcHelper::prepareToReturn(_session,__os,rmiReturn);\n\n"
					<<"	__os->setUseBitMark(true);\n\n"
					<<cppCallBackWrite.content<<"\n"
					<<"	CRpcHelper::toReturn(_session,__os);\n\n"
					<<"}\n\n";	
	}

	callBackDef<<"}\n\n"<<"\n";
	headInterfaceStream<<callBackDef.content<<"\n";
	cppInterfaceStream<<cppCallBack.content;

	//*************************Server callBack end******************************

	//======================Server interface class struct and onCall Def start==============

	CSGStream cppClassStruct;
	CSGStream cppClassOncall;

	cppClassStruct<<"Message::"<<interfaceClassName<<"::"<<interfaceClassName<<"()\n"
					<<"{\n"
					<<"	SRMIInfo rmiInfo;\n"
					<<"	rmiInfo.identify=\""<<interfaceClassName<<"\";\n\n";
	cppClassOncall<<"csg::ERMIDispatchResult Message::"<<interfaceClassName<<"::__onCall(const CSessionPtr& session,const SRMICall& rmiCall,CSerializeStream& __is)\n"
					<<"{\n"
					<<"	switch(rmiCall.rpcId)\n"
					<<"	{\n";

	for(int i=0;i<interfaceList.size();i++){
		CSGInterface tmp=interfaceList[i];
		std::string functionName=tmp.funcName;
		outputInfo<<"functionName:"<<tmp.funcName<<",rpcid:"<<toStr(tmp.rpcId)<<"\n";

		//class def
		std::string srvCB=srvCBprefix+functionName;
		std::string srvCBPtr=srvCB+"_Ptr";
		std::string cliCB=cliCBprefix+functionName;
		std::string cliCBPtr=cliCB+"_Ptr";
		CSGStream cppProxyCallBackRead;
		CSGStream cppProxyCallBackParam;
		CSGStream cppProxyReturnParam;

		CSGStream cppProxyFunction;
		CSGStream cppProxyCallWrite;
	

		cppClassStruct<<"	rmiInfo.operation=\""<<functionName<<"\";\n"
						<<"	addRMIInfo("<<toStr(tmp.rpcId)<<",rmiInfo);\n"
						<<"	addRpcId("<<toStr(tmp.rpcId)<<");\n\n";
		cppClassOncall<<"		case "<<toStr(tmp.rpcId)<<":\n"
						<<"		{\n"
						<<"			return __"<<functionName<<"_async(session,rmiCall,__is);\n"
						<<"		}\n"
						<<"		break;\n";
	}
	cppClassStruct<<"}\n\n";	
	cppClassOncall<<"		default:\n"
					<<"		break;\n"
					<<"	}\n\n"
					<<"	return ERMIDispatchObjectNotExist;\n\n"
					<<"}\n\n";

	cppInterfaceStream<<cppClassStruct.content;
	cppInterfaceStream<<cppClassOncall.content;

	//*********************Server interface class struct  and onCall Def end*************

	//======================Server interface class start==============

	CSGStream classDef;
	CSGStream cppClassInterface;
	classDef<<"namespace "<<mNamespace<<"\n"
				<<"{\n"
				<<"	class "<<interfaceClassName<<":public virtual CRMIObject\n"
				<<"	{\n"
				<<"	public:\n"
				<<"		"<<interfaceClassName<<"();\n\n"
				<<"		virtual ERMIDispatchResult __onCall(const CSessionPtr&,const SRMICall&,CSerializeStream&);\n\n";

	for(int i=0;i<interfaceList.size();i++){
		CSGInterface tmp=interfaceList[i];
		std::string functionName=tmp.funcName;

		//class def
		std::string srvCB=srvCBprefix+functionName;
		std::string srvCBPtr=srvCB+"_Ptr";
		std::string cliCB=cliCBprefix+functionName;
		std::string cliCBPtr=cliCB+"_Ptr";

		CSGStream functionDefStream;
		CSGStream cppClassProxyParam;

		functionDefStream<<"		ERMIDispatchResult __"<<functionName<<"_async("<<"const CSessionPtr&,const SRMICall&,CSerializeStream&);\n\n";
		functionDefStream<<"		virtual void "<<functionName<<"_async(const CSessionPtr& ,"<<mNamespace<<"::"<<srvCBPtr<<"&";

		cppClassInterface<<"csg::ERMIDispatchResult Message::"<<interfaceName<<"::__"<<functionName<<"_async(const CSessionPtr& session,const SRMICall& rmiCall,CSerializeStream& __is)\n"
						<<"{\n";

		isFirstOutParam=true;
		isFirstParam=true;
		for(int j=0;j<tmp.paramList.size();j++){
			CSGInterfaceParam param=tmp.paramList[j];
			bool isOut=param.isOut;
			std::string type=param.type;
			std::string identify=param.identify;

			if(!isOut){
				functionDefStream<<","<<type;
				cppClassInterface<<"	"<<param.type<<" "<<param.identify<<";\n"
							<<"	__is.read("<<param.identify<<");\n";
				cppClassProxyParam<<","<<param.identify;
			}
		}
		functionDefStream<<")=0;\n\n";
		classDef<<functionDefStream.content;

		cppClassInterface<<"	"<<srvCBPtr<<" cb=new "<<srvCB<<"();\n"
						<<"	cb->setSession(session,rmiCall);\n"
						<<"	"<<functionName<<"_async(session,cb"<<cppClassProxyParam.content<<");\n\n"
						<<"	return ERMIDispatchResultOk;\n\n"
						<<"}\n\n";
	}

	classDef<<"	};\n"
			<<"}\n";
	headInterfaceStream<<classDef.content<<"\n";
	cppInterfaceStream<<cppClassInterface.content;
	//*************************SServer interface class end******************************


	//======================proxy callBack start==============

	CSGStream proxyCallBackDef;
	CSGStream cppProxyCallBack;
	proxyCallBackDef<<"namespace "<<proxyNamespace<<"\n"
					<<"{\n";

	for(int i=0;i<interfaceList.size();i++){
		CSGInterface tmp=interfaceList[i];
		std::string functionName=tmp.funcName;

		//class def
		std::string srvCB=srvCBprefix+functionName;
		std::string srvCBPtr=srvCB+"_Ptr";
		std::string cliCB=cliCBprefix+functionName;
		std::string cliCBPtr=cliCB+"_Ptr";

		CSGStream cppProxyCallBackParam;
		CSGStream cppProxyReturnParam;

		proxyCallBackDef<<"	class "<<cliCB<<":public virtual CRMIProxyCallBackObject\n"
						<<"	{\n"
						<<"	public:\n"
						<<"		virtual void response(";
		cppProxyCallBack<<"void csg_proxy::"<<cliCB<<"::__response(CSerializeStream& __is)\n"
						<<"{\n";


		isFirstOutParam=true;
		isFirstParam=true;
		
		for(int j=0;j<tmp.paramList.size();j++){
			CSGInterfaceParam tmpParam=tmp.paramList[j];

			if(tmpParam.isOut){
				if(isFirstOutParam){
					proxyCallBackDef<<tmpParam.type<<" "<<tmpParam.identify;
					cppProxyReturnParam<<tmpParam.identify;
					isFirstOutParam=false;
				}else{
					proxyCallBackDef<<","<<tmpParam.type<<" "<<tmpParam.identify;
					cppProxyReturnParam<<","<<tmpParam.identify;
				}
				cppProxyCallBackParam<<"	"<<tmpParam.type<<" "<<tmpParam.identify<<";\n"
								<<"	__is.read("<<tmpParam.identify<<");\n";
			}
		}
		proxyCallBackDef<<")=0;\n"
						<<"	protected:\n"
						<<"		virtual void __response(CSerializeStream& __is);\n"
						<<"	};\n"
						<<"	typedef CSmartPointShare<"<<cliCB<<"> "<<cliCBPtr<<";\n\n";
		cppProxyCallBack<<cppProxyCallBackParam.content
						<<"\n"
						<<"	response("<<cppProxyReturnParam.content<<");\n\n"
						<<"}\n\n";

	}

	proxyCallBackDef<<"}\n";

	headInterfaceStream<<proxyCallBackDef.content<<"\n";
	cppInterfaceStream<<cppProxyCallBack.content;
	//*************************proxy callBack end******************************

	//======================proxy class start==============

	CSGStream proxyClassDef;
	CSGStream cppProxyClass;
	CSGStream cppProxyFunction;

	proxyClassDef<<"namespace "<<proxyNamespace<<"\n"
					<<"{\n"
					<<"	class "<<interfaceClassName<<":public virtual CRMIProxyObject\n"
					<<"	{\n"
					<<"	public:\n"
					<<"		"<<interfaceClassName<<"();\n";
	cppProxyClass<<"csg_proxy::"<<interfaceClassName<<"::"<<interfaceClassName<<"()\n"
					<<"{\n"
					<<"}\n\n";

	for(int i=0;i<interfaceList.size();i++){
		CSGInterface tmp=interfaceList[i];
		std::string functionName=tmp.funcName;

		//class def
		std::string srvCB=srvCBprefix+functionName;
		std::string srvCBPtr=srvCB+"_Ptr";
		std::string cliCB=cliCBprefix+functionName;
		std::string cliCBPtr=cliCB+"_Ptr";

		CSGStream cppProxyCallWrite;

		//proxy headfile code
		proxyClassDef<<"		void "<<functionName<<"_async(const CSessionPtr&,const "<<cliCBPtr<<"&";
		cppProxyFunction<<"void csg_proxy::"<<interfaceName<<"::"<<functionName<<"_async(const CSessionPtr& session,const "<<cliCBPtr<<"& cb";

		isFirstOutParam=true;
		isFirstParam=true;
		
		for(int j=0;j<tmp.paramList.size();j++){
			CSGInterfaceParam tmpParam=tmp.paramList[j];

			if(!tmpParam.isOut){
				proxyClassDef<<","<<tmpParam.type;
				cppProxyFunction<<","<<tmpParam.type<<" "<<tmpParam.identify;
				cppProxyCallWrite<<"	__os->write("<<tmpParam.identify<<");\n";
			}
		}
		proxyClassDef<<");\n\n";
		cppProxyFunction<<")\n"
						<<"{\n"
						<<"	csg::SRMICall call;\n"
						<<"	call.rpcId="<<toStr(tmp.rpcId)<<";\n\n"
						<<"	CAutoSerializeStream __os(CSerializeStreamPool::instance()->newObject());\n"
						<<"	CRMIObjectBindPtr objectBind=NULL;\n"
						<<"	CRpcHelper::prepareToCall(session,__os,call,cb,objectBind);\n\n"
						<<"	__os->setUseBitMark(true);\n"
						<<cppProxyCallWrite.content<<"\n"
						<<"	CRpcHelper::toCall(session,__os,objectBind"<<");\n\n"
						<<"}\n\n";
	}

	proxyClassDef<<"	};\n"
				<<"}\n";

	headInterfaceStream<<proxyClassDef.content<<"\n";
	cppInterfaceStream<<cppProxyClass.content;
	cppInterfaceStream<<cppProxyFunction.content;
	//**********************proxy class start end******************************

};

define_line: TOKEN_IFNDEF TOKEN_IFNDEF_FILE TOKEN_IFNDEF_END {
	headDefStream<<"#ifndef "<<$2<<"\n";
}|TOKEN_IFDEF TOKEN_IFDEF_FILE TOKEN_IFDEF_END {
	headDefStream<<"#define "<<$2<<"\n";
}|TOKEN_ENDIF {
	endDefStream<<$1<<"\n";
};

comment: TOKEN_COMMENT_START TOKEN_COMMENT_FILE TOKEN_COMMENT_END {
	outputInfo<<"yacc match comment:"<<BOLDGREEN<<"//"<<$2<<"\r"<<RESET<<"\n";
}|TOKEN_COMMENT_EX_START TOKEN_COMMENT_EX_FILE TOKEN_COMMENT_EX_END {
	outputInfo<<"yacc match commentex:"<<BOLDGREEN<<"/**"<<$2<<"\r"<<RESET<<"\n";
}
;

include: TOKEN_INCLUDE_START TOKEN_INCLUDE_SIGN TOKEN_INCLUDE_FILE TOKEN_INCLUDE_END {
	headIncludeStream<<"#include \""+$3+"\"\n";
};

enum : TOKEN_ENUM_START TOKEN_ENUM_CONTENT {
	
	std::string enumName=$1;
	int minEnumValue=999999;
	int maxEnumValue=-99999;
	int lastValue=-1;
	//generated enum code
	CSGStream enumHeadfileStream;
	enumHeadfileStream<<"	enum "+enumName+" {\n";
	for(int i=0;i<$2.size();i++){
		CSGEnum tmp=$2[i];
		enumHeadfileStream<<"		"<<tmp.identify;
		if(0!=tmp.value){
			enumHeadfileStream<<" = "<<toStr(tmp.value);
			if(lastValue>=tmp.value){
				std::cerr<<"error enum value,less than last one,enum name:"<<enumName<<",identify:"<<tmp.identify<<std::endl;
				globalError=true;
			}
			lastValue=tmp.value;
		}else{
			lastValue++;
		}
		enumHeadfileStream<<",\n";
		minEnumValue=minEnumValue<lastValue?minEnumValue:lastValue;
		maxEnumValue=maxEnumValue>lastValue?maxEnumValue:lastValue;
	}
	enumHeadfileStream<<"	};\n\n";
	
	//generated serialize code
	enumHeadfileStream<<"	void _csg_write(CSerializeStream& __os,"<<enumName<<" __enumType);\n";
	enumHeadfileStream<<"	void _csg_read(CSerializeStream& __is,"<<enumName<<"& __enumType);\n";
	enumHeadfileStream<<"\n\n";
	
	headfileContentStream<<enumHeadfileStream.content;
	
	//generated cpp serialize code
	std::string typeName=maxEnumValue>255?"int":"byte_t";
	
	CSGStream enumCppStream;
	
	enumCppStream<<"void Message::_csg_write(CSerializeStream& __os,"<<enumName<<" __enumType){\n"
				<<"	__os.write(static_cast<"<<typeName<<">(__enumType));\n}\n\n";
	enumCppStream<<"void Message::_csg_read(CSerializeStream& __is,"<<enumName<<"& __enumType){\n"
				<<"	"<<typeName<<" value;\n"
				<<"	__is.read(value);\n"
				<<"	__enumType=static_cast<"<<enumName<<">(value);\n"
				<<"	if(__enumType>"<<toStr(maxEnumValue)<<"||__enumType<"<<toStr(minEnumValue)<<")\n"
				<<"	{\n"
				<<"		throw CException(\"ExceptionCodeSerialize\",ExceptionCodeSerialize);\n"
				<<"	}\n"
				<<"}\n\n";
	
	cppContentStream<<enumCppStream.content;

};
struct: TOKEN_ID_START TOKEN_ID_NUM TOKEN_ID_END TOKEN_STRUCT_START TOKEN_STRUCT_NAME TOKEN_STRUCT_CONTENT {
	std::string structName=$5;
	structVec.push_back(structName);
	int idNum=$2;
	CSGStream structHeadfileStream;
	structHeadfileStream<<"	class "<<structName<<"\n"
						<<"		:public virtual csg::IMsgBase {\n"
						<<"	public:\n";

	std::string initDef="	";
	std::string readDef;
	std::string writeDef;
	
	std::string operatorEq="	";
	std::string operatorEqEq;
	std::string operatorless;
	
	for(int i=0;i<$6.size();i++){
		CSGStruct tmp=$6[i];
		std::string valueType;
		if("int"==tmp.type){
			initDef+=tmp.identify+"=0;\n	";
		}else if("double"==tmp.type){
			initDef+=tmp.identify+"=0.0;\n	";
		}else if("float"==tmp.type){
			initDef+=tmp.identify+"=0.0f;\n	";
		}else if("bool"==tmp.type){
			initDef+=tmp.identify+"=false;\n	";
		}else if("long"==tmp.type){
			initDef+=tmp.identify+"=0;\n	";
			valueType="long64_t";
		}else if("string"==tmp.type){
			initDef+=tmp.identify+"=\"\";\n	";
			valueType="std::string";
		}else{
			initDef+=tmp.identify+".clear();\n	"; 
			valueType="std::"+tmp.type;
		};
		if(""==valueType){
			valueType=tmp.type;
		}

		structHeadfileStream<<"		"<<valueType<<"  "<<tmp.identify<<";\n";

		readDef+="	__is.read("+tmp.identify+");\n";
		writeDef+="	__os.write("+tmp.identify+");\n";

		operatorEq+=tmp.identify+" = "+"__other."+tmp.identify+";\n	";

		operatorEqEq+="	if("+tmp.identify+" !="+" __other."+tmp.identify+")\n"
						+"	{\n"+"		return true;\n	}\n";

		operatorless+="	if("+tmp.identify+" <"+" __other."+tmp.identify+")\n"
						+"	{\n"+"		return true;\n	}\n";
	}
	
	structHeadfileStream<<"\n";
	structHeadfileStream<<"	public:\n"
						<<"		"<<structName<<"();\n"
						<<"		"<<structName<<"(const "<<structName<<" &);\n"
						<<"		"<<structName<<"& operator=(const "<<structName<<" &);\n";
	structHeadfileStream<<"\n";
	structHeadfileStream<<"	public:\n";
	structHeadfileStream<<"		bool operator==(const "<<structName<<" &)const;\n"
						<<"		bool operator!=(const "<<structName<<" &)const;\n"
						<<"		bool operator<(const "<<structName<<" &)const;\n\n"
						<<"		void _csg_init();\n"
						<<"		void _csg_read(CSerializeStream&);\n"
						<<"		void _csg_write(CSerializeStream&)const;\n\n"
						<<"		virtual IMsgBase* clone();\n"
						<<"		virtual int getType()const;\n";
	structHeadfileStream<<"	private:\n"
						<<"		static const int _msgType = "<<toStr(idNum)<<";\n";
						
	structHeadfileStream<<"	};\n\n";
	structHeadfileStream<<"	typedef csg::CSmartPointShare<"<<structName<<"> "<<structName<<"_Ptr;\n\n";
	
	headfileContentStream<<structHeadfileStream.content;

	//generated cpp code
	CSGStream structCppStream;
	std::string constructioncpp="";

	//construcion
	structCppStream<<"Message::"<<structName<<"::"<<structName<<"()\n"
					<<"{\n"
					<<"	_csg_init();\n"
					<<"}\n\n";

	structCppStream<<"Message::"<<structName<<"::"<<structName<<"(const "<<structName<<"& __other)\n"
					<<"{\n"
					<<"	if(this==&__other)\n"
					<<"	{\n"
					<<"		return;\n"
					<<"	}\n"
					<<"	*this=__other;\n"
					<<"}\n\n";

	structCppStream<<"Message::"<<structName<<"& Message::"<<structName<<"::operator=(const "<<structName<<"& __other)\n"
					<<"{\n"
					<<"	if(this==&__other)\n"
					<<"	{\n"
					<<"		return *this;\n"
					<<"	}\n"
					<<"	IMsgBase::operator=(__other);\n"
					<<operatorEq
					<<"return *this;\n"
					<<"}\n\n";

	structCppStream<<"int Message::"<<structName<<"::getType()const{\n"
					<<"	return _msgType;\n"
					<<"}\n\n";
	
	structCppStream<<"csg::IMsgBase* Message::"<<structName<<"::clone()\n"
					<<"{\n"
					<<"	return new "<<structName<<"(*this);\n"
					<<"}\n\n";
				
	structCppStream<<"bool Message::"<<structName<<"::operator==(const "<<structName<<"& __other)const\n"
					<<"{\n"
					<<"	return !operator!=(__other);\n"
					<<"}\n\n";

	structCppStream<<"bool Message::"<<structName<<"::operator!=(const "<<structName<<"& __other)const\n"
					<<"{\n"
					<<"	if(this==&__other)\n"
					<<"	{\n"
					<<"		return false;\n"
					<<"	}\n"
					<<operatorEqEq
					<<"	return false;\n"
					<<"}\n\n";

	structCppStream<<"bool Message::"<<structName<<"::operator<(const "<<structName<<"& __other)const\n"
					<<"{\n"
					<<"	if(this==&__other)\n"
					<<"	{\n"
					<<"		return false;\n"
					<<"	}\n"
					<<operatorless
					<<"	return false;\n"
					<<"}\n\n";



	structCppStream<<"void Message::"<<structName<<"::_csg_init(){\n"
					<<initDef<<"\n};\n\n";	
	structCppStream<<"void Message::"<<structName<<"::_csg_read(CSerializeStream& __is){\n"
					<<readDef<<"};\n\n";
	structCppStream<<"void Message::"<<structName<<"::_csg_write(CSerializeStream& __os)const{\n"
					<<writeDef<<"};\n\n";
					
	cppContentStream<<structCppStream.content;
};


%%


std::string toStr(int i){
	std::stringstream ss;
	ss<<i;
	return ss.str();

};


void csgInfo(std::string type,std::string s){
	bool defineType=false;
	if(type=="ifndef"){
		//defineType=true ;
	}		
	
	bool unknowB=false;
	if("table"==type)
		unknowB=true;
	if("newline"==type)
		unknowB=true;
	if("blank"==type)
		unknowB=true;
	if("unknow"==type)
		unknowB=true;

	if(unknowB){
		//return ;
	
	}
	if(defineType){
		//return ;
	}
	
	outputInfo<<YELLOW<<"csgl:"<<type<<","<<BOLDYELLOW<<"["<<s<<"]"<<RESET<<RESET<<"\n";
}

void csgOutputCppHead(std::string fileName){
	ofscpp<<"#include "<<"\""<<fileName+"\.h\"\n";
	ofscpp<<"#include \"engine/rpc/rpcHelper.h\"\n";
	ofscpp<<"using namespace csg;\n";
	ofscpp<<"using namespace Message;\n\n";
}


void csgOutputHead(){
	ofs<<csgDefStr<<"\n";
	ofs<<headDefStream.content<<"\n";
	ofs<<includeCsgFile<<"\n";
	ofs<<headIncludeStream.content<<"\n";
	ofs<<"using namespace csg;"<<"\n\n";
	ofs<<"namespace Message {"<<"\n\n";
	
}

void csgOutputEnd(std::string fileName){
	//regist struct
	std::string registClass="C"+fileName;
	ofs<<"	class "<<registClass<<"{\n"
		<<"	public:\n"
		<<"		static void regist();\n"
		<<"	};\n";
	ofs<<"\n}\n\n";//namespace

	//interface

	ofs<<headInterfaceStream.content<<"\n";

	ofs<<endDefStream.content<<"\n";

	CSGStream cppregistStream;
	cppregistStream<<"void Message::"<<registClass<<"::regist()\n"
					<<"{\n";
	for(int i=0;i<structVec.size();i++){
		cppregistStream<<"	csg::CMsgManager::instance()->regist(new "<<structVec[i]<<"());\n";
	}
	cppregistStream<<"}\n";
	cppContentStream<<cppregistStream.content;
}



void yyerror(const char* s){
	std::cerr<<"\033[31m"<<"get error:["<<s<<"]\033[0m"<<std::endl;
}


int main(int argc,char **argv){
	
	//get csl file
	if(argc<4){
		std::cerr<<RED<<"no enough param,first is csgl file ,next is outfile dir"<<RESET<<std::endl;
	}
	std::string inputfile(*(argv+3));
	std::string inputfileName=inputfile+".csgl";
	std::string inputDir(*(argv+1));
	std::string outputDir(*(argv+2));
	std::string inputfileCsg=inputDir+inputfileName;
	std::string outputHeadFileName=outputDir+inputfile+".h";
	std::string outputCppFileName=outputDir+inputfile+".cpp";

	std::cout<<"input file is "<<YELLOW<<inputfileCsg<<RESET<<std::endl;
	std::cout<<"output dir is "<<YELLOW<<outputDir<<RESET<<std::endl;
	std::cout<<"output headfile is "<<GREEN<<outputHeadFileName<<RESET<<std::endl;
	std::cout<<"output cppfile is "<<GREEN<<outputCppFileName<<RESET<<std::endl;

	FILE *fp=fopen(inputfileCsg.c_str(),"r");
	if(NULL==fp){
		std::cerr<<RED<<"can't open file "<<inputfileCsg<<RESET<<std::endl;
		return 0;
	}
	
	ofs.open(outputHeadFileName);
	ofscpp.open(outputCppFileName);

	if(!ofs.is_open()){
		std::cerr<<RED<<"can't open file "<<outputHeadFileName<<RESET<<std::endl;
		return 0;
	}
	if(!ofscpp.is_open()){
		std::cerr<<RED<<"can't open file "<<outputCppFileName<<RESET<<std::endl;
		return 0;
	}

	yyin=fp;
	yyparse();
	fclose(fp);

	if(globalError){
		return 0;
	}
	std::cout<<GREEN<<"start generated head file "<<inputfileName<<RESET<<std::endl;
	//generated headfile
	csgOutputHead();
	ofs<<headfileContentStream.content<<std::endl;


	csgOutputEnd(inputfile);


	std::cout<<GREEN<<"start generated cpp file "<<inputfileName<<RESET<<std::endl;
	//generated cppfile
	csgOutputCppHead(inputfile);
	ofscpp<<cppContentStream.content<<std::endl;
	ofscpp<<cppInterfaceStream.content<<"\n";

	std::cout<<BOLDGREEN<<"build csgl file success!"<<RESET<<std::endl;
	
	ofs.close();
	ofscpp.close();
	return 0;
}



