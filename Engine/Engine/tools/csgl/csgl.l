%{
#include "csgl.hpp"
#include "csgl.tab.h"
#include <assert.h>


extern void csgInfo(std::string type,std::string s);

%}

space [ \t]
blank {space}+
letter [a-zA-Z_]+
digit	[0-9]
num [1-9]+{digit}*
identify {letter}+({letter}|{digit})*
headfileidentify ({identify}|\.|\/)*
newline	[\r\n]
endline {space}*{newline}+
specialletter	[\#\&\^\-\?\/\+\*\(\)\{\}\<\>\\\\;\.\|\$\%"]
specialletterEx	[\#\&\^\-\?\+\*\(\)\{\}\<\>\\\\;\.\|\$\%"]
allletter ({identify}|{blank}|{digit}|{specialletter})*
allletterEx ({identify}|{blank}|{digit}|{specialletterEx})*

INT "int"
FLOAT "float"
DOUBLE "double"
BOOL "bool"
LONG "long"
STRING "string"
VECTOR "vector"
MAP "map"
VOID "void"
INTERFACE "interface"

csgKey ({INT}|{BOOL}|{LONG}|{STRING})



%s _IFNDEF_START_ _IFNDEF_FILE_ _IFNDEF_END_
%s _IFDEF_START_ _IFDEF_FILE_ _IFDEF_END_
%s _COMMENT_START_ _COMMENT_FILE_ _COMMENT_EX_START_ _COMMENT_EX_FILE_ 
%s _INCLUDE_START_ _INCLUDE_FILE_ _INCLUDE_END_
%s _ENUM_START_ _ENUM_START_EX_ _ENUM_CONTENT_ _ENUM_CONTENT_VALUE_ _ENUM_CONTENT_VALUE_NUM_ _ENUM_CONTENT_VALUE_END_
%s _STRUCT_START_ _STRUCT_NAME_ _STRUCT_CONTENT_TYPE_ _STRUCT_CONTENT_TYPE_IDENTIFY_ _STRUCT_CONTENT_TYPE_IDENTIFY_END_
%s _ID_START_ _ID_NUM_ _ID_END_
%s _INTERFACE_START_ _INTERFACE_NAME_ _INTERFACE_CONTENT_ _INTERFACE_ID_START_ _INTERFACE_ID_NUM_ _INTERFACE_ID_END_ _INTERFACE_FUNCTION_NAME_ _INTERFACE_FUNCTION_END_ _INTERFACE_PARAM_ _INTERFACE_PARAM_TYPE_ _INTERFACE_PARAM_NAME_ _INTERFACE_PARAM_IDENTIFY_ _INTERFACE_PARAM_END_

%%

^{space}*"#ifndef"{blank}+	{
	csgInfo("ifndef","start");	
	BEGIN _IFNDEF_START_;
	yylval.clear();
	return TOKEN_IFNDEF;
};

<_IFNDEF_START_>{identify} {
	csgInfo("ifndef","file start");
	BEGIN _IFNDEF_FILE_;
	yylval.t_string=yytext;
	csgInfo("ifndefv",yylval.t_string);
	return TOKEN_IFNDEF_FILE;
};

<_IFNDEF_FILE_>{newline}+ {
	csgInfo("ifndef","file end by newline");
	BEGIN INITIAL;
	return TOKEN_IFNDEF_END;
};

<_IFNDEF_FILE_>{blank}+ {
	csgInfo("ifndef","file end by space");
	BEGIN INITIAL;
	return TOKEN_IFNDEF_END;
};

<_IFNDEF_FILE_>{endline}+ {
	csgInfo("ifndef","file end by endline");
	BEGIN INITIAL;
	return TOKEN_IFNDEF_END;
};

^{space}*"#define"{blank}+	{
	csgInfo("define","start");	
	BEGIN _IFDEF_START_;
	yylval.clear();
	return TOKEN_IFDEF;
};

<_IFDEF_START_>{identify} {
	csgInfo("define","file start");
	BEGIN _IFDEF_FILE_;
	yylval.t_string=yytext;
	csgInfo("definev",yylval.t_string);
	return TOKEN_IFDEF_FILE;
};

<_IFDEF_FILE_>{newline}+ {
	csgInfo("define","file end by newline");
	BEGIN INITIAL;
	return TOKEN_IFDEF_END;
};

<_IFDEF_FILE_>{blank}+ {
	csgInfo("define","file end by space");
	BEGIN INITIAL;
	return TOKEN_IFDEF_END;
};

<_IFDEF_FILE_>{endline}+ {
	csgInfo("define","file end by endline");
	BEGIN INITIAL;
	return TOKEN_IFDEF_END;
};

^{space}*"#endif"{blank}*	{
	csgInfo("endif","file end by space");	
	yylval.clear();
	yylval.t_string=yytext;
	return TOKEN_ENDIF;
};


^{space}*"#endif"{newline}+	{
	csgInfo("endif","file end by newline");	
	yylval.clear();
	yylval.t_string=yytext;
	return TOKEN_ENDIF;
};

^{space}*"#include"{blank}+ {
	BEGIN _INCLUDE_START_;
	csgInfo("include","start");
	yylval.clear();
	return TOKEN_INCLUDE_START;
};

<_INCLUDE_START_>[\"<] {
	BEGIN _INCLUDE_FILE_;
	csgInfo("include","file start");
	//yylval.t_string=yytext[0];
	return TOKEN_INCLUDE_SIGN;
};
<_INCLUDE_FILE_>{headfileidentify}* {
	BEGIN _INCLUDE_END_; 	
	yylval.t_string=yytext;
	return TOKEN_INCLUDE_FILE;
};
<_INCLUDE_END_>[\">]{endline}* {
	BEGIN INITIAL;
	//yylval.t_string+=yytext;
	csgInfo("includev",yylval.t_string);
	csgInfo("include","end");
	return TOKEN_INCLUDE_END;
};	

^{space}*"enum"{blank}+ {
	BEGIN _ENUM_START_;
	csgInfo("enum","start");
	csgInfo("enumv",yytext);
	yylval.clear();
}; 

<_ENUM_START_>{identify} {
	BEGIN _ENUM_START_EX_;
	csgInfo("enum getIdentify",yytext);
	yylval.t_string=yytext;
	return TOKEN_ENUM_START;

};
<_ENUM_START_EX_>{blank}*"{" {
	BEGIN _ENUM_CONTENT_;
	csgInfo("enum","start identity");
};


<_ENUM_CONTENT_>"}"{blank}*";" {
	BEGIN INITIAL;
	csgInfo("enum","file end");
	return TOKEN_ENUM_CONTENT;
};

<_ENUM_CONTENT_>{identify} {
	BEGIN _ENUM_CONTENT_VALUE_;
	csgInfo("enumv",yytext);
	yylval.tmpEnumKey.clear();
	yylval.tmpEnumKey.identify=yytext;

};

<_ENUM_CONTENT_VALUE_>"," {
	BEGIN _ENUM_CONTENT_;
	yylval.te_list.push_back(yylval.tmpEnumKey);
	csgInfo("enume",yylval.tmpEnumKey.identify);
};
<_ENUM_CONTENT_VALUE_>"=" {
	BEGIN _ENUM_CONTENT_VALUE_NUM_;
	csgInfo("enume","have def value");
};

<_ENUM_CONTENT_VALUE_NUM_>{digit}+ {
	BEGIN _ENUM_CONTENT_VALUE_END_;
	csgInfo("enume","get value");
	yylval.tmpEnumKey.value=atoi(yytext);
};

<_ENUM_CONTENT_VALUE_END_>"," {
	BEGIN _ENUM_CONTENT_;
	yylval.te_list.push_back(yylval.tmpEnumKey);
	csgInfo("enume",yylval.tmpEnumKey.identify);
};




^{space}*"[\"id\":" {
	BEGIN _ID_START_;
	csgInfo("id","start");
	yylval.clear();
	return TOKEN_ID_START;
};
<_ID_START_>{num} {
	BEGIN _ID_NUM_;
	yylval.id=atoi(yytext);	
	csgInfo("idv",yytext);
	return TOKEN_ID_NUM;
};
<_ID_NUM_>"]" {
	//BEGIN INITIAL;
	BEGIN _ID_END_;
	csgInfo("id","end");
	return TOKEN_ID_END;
};


<_ID_END_>{space}*"struct"{blank}+ {
	BEGIN _STRUCT_START_;
	csgInfo("struct","start");
	//yylval.clear();
	return TOKEN_STRUCT_START;
};
<_STRUCT_START_>{identify}+ {
	BEGIN _STRUCT_NAME_;
	csgInfo("struct",yytext);
	yylval.t_string=yytext;
	return TOKEN_STRUCT_NAME;
};
<_STRUCT_NAME_>{blank}*"{" {
	BEGIN _STRUCT_CONTENT_TYPE_;
	yylval.tstruct_list.clear();
	csgInfo("struct","get identity");
	
};

<_STRUCT_CONTENT_TYPE_>{blank}*"}"{blank}*";" {
	BEGIN INITIAL;
	csgInfo("struct","end");
	return TOKEN_STRUCT_CONTENT;
};

<_STRUCT_CONTENT_TYPE_>{csgKey} {
	BEGIN _STRUCT_CONTENT_TYPE_IDENTIFY_;
	csgInfo("structtype",yytext);
	yylval.tmpStructKey.type=yytext;
};
<_STRUCT_CONTENT_TYPE_>"vector<"{csgKey}">" {
	BEGIN _STRUCT_CONTENT_TYPE_IDENTIFY_;
	csgInfo("structv",yytext);
	yylval.tmpStructKey.type=yytext;
};

<_STRUCT_CONTENT_TYPE_>"map<"{csgKey}","{csgKey}">" {
	BEGIN _STRUCT_CONTENT_TYPE_IDENTIFY_;
	csgInfo("structv",yytext);
	yylval.tmpStructKey.type=yytext;
};
<_STRUCT_CONTENT_TYPE_IDENTIFY_>{identify} {
	BEGIN _STRUCT_CONTENT_TYPE_IDENTIFY_END_;
	csgInfo("structIdentity",yytext);
	yylval.tmpStructKey.identify=yytext;
	yylval.tstruct_list.push_back(yylval.tmpStructKey);
};
<_STRUCT_CONTENT_TYPE_IDENTIFY_END_>";" {
	BEGIN _STRUCT_CONTENT_TYPE_;
}


^{space}*"interface"{blank}+ {
	BEGIN _INTERFACE_START_;
	csgInfo("interface","start");
	yylval.clear();
};

<_INTERFACE_START_>{identify} {
	BEGIN _INTERFACE_NAME_;
	yylval.t_string=yytext;
	csgInfo("interfacev",yytext);
	return TOKEN_INTERFACE_NAME;
};

<_INTERFACE_NAME_>"{"{endline}* {
	BEGIN _INTERFACE_CONTENT_;
	csgInfo("interface","start get function");
};

<_INTERFACE_CONTENT_>{space}*"}"{blank}*";"{endline}* {
	BEGIN INITIAL;
	csgInfo("interface","all end");
	return TOKEN_INTERFACE_CONTENT;
};

<_INTERFACE_CONTENT_>^{blank}*"[\"rpcid\":" {
	BEGIN _INTERFACE_ID_START_;
	csgInfo("interface","rpcid");
	yylval.tmpInterfaceKey.clear();
};
<_INTERFACE_ID_START_>{num} {
	BEGIN _INTERFACE_ID_NUM_;
	csgInfo("interfacev",yytext);
	yylval.tmpInterfaceKey.rpcId=atoi(yytext);
};
<_INTERFACE_ID_NUM_>"]"{blank}+"void"{blank}+ {
	BEGIN _INTERFACE_FUNCTION_NAME_;
	csgInfo("interface","id end");
};

<_INTERFACE_FUNCTION_NAME_>{identify} {
	BEGIN _INTERFACE_FUNCTION_END_;
	yylval.tmpInterfaceKey.funcName=yytext;
	csgInfo("interfacefn",yytext);
};

<_INTERFACE_FUNCTION_END_>{blank}*"("{blank}* {
	BEGIN _INTERFACE_PARAM_;
	csgInfo("interfaceparam","start");
};
<_INTERFACE_PARAM_>{blank}*")"{blank}* {
	BEGIN _INTERFACE_PARAM_END_;
	csgInfo("interfaceparam","near end");
}
<_INTERFACE_PARAM_END_>{blank}*";"{endline}* {
	BEGIN _INTERFACE_CONTENT_;
	csgInfo("interface","end a function");
	yylval.tinterface_list.push_back(yylval.tmpInterfaceKey);
	yylval.tmpInterfaceParamKey.clear();
	yylval.tmpInterfaceKey.clear();
};

<_INTERFACE_PARAM_>{csgKey} {
	BEGIN _INTERFACE_PARAM_TYPE_;
	csgInfo("interfaceparamtype",yytext);
	yylval.tmpInterfaceParamKey.type=yytext;
};	

<_INTERFACE_PARAM_>"out"{blank}+ {
	if(yylval.tmpInterfaceParamKey.isOut){
		csgInfo("interface","more than one out in one param");
	}
	csgInfo("interface","get a out param");
	yylval.tmpInterfaceParamKey.isOut=true;
};

<_INTERFACE_PARAM_TYPE_>{identify} {
	BEGIN _INTERFACE_PARAM_IDENTIFY_;
	yylval.tmpInterfaceParamKey.identify=yytext;
	csgInfo("interfaceparamidentify",yytext);
};

<_INTERFACE_PARAM_IDENTIFY_>"," {
	BEGIN _INTERFACE_PARAM_;
	yylval.tmpInterfaceKey.paramList.push_back(yylval.tmpInterfaceParamKey);
	yylval.tmpInterfaceParamKey.clear();
	csgInfo("interface","get a complete param");
};

<_INTERFACE_PARAM_IDENTIFY_>")" {
	BEGIN _INTERFACE_PARAM_END_;
	yylval.tmpInterfaceKey.paramList.push_back(yylval.tmpInterfaceParamKey);
	yylval.tmpInterfaceParamKey.clear();
};






"//"	{
		BEGIN _COMMENT_START_;
		yylval.clear();	
		csgInfo("comment","start");
		return TOKEN_COMMENT_START;
	};
<_COMMENT_START_>{allletter}*	{
		BEGIN _COMMENT_FILE_;
		csgInfo("comment","file ");
		yylval.t_string=yytext;
		csgInfo("commentv",yylval.t_string);
		return TOKEN_COMMENT_FILE;
	};
<_COMMENT_FILE_>{newline}? {
		BEGIN INITIAL;
		csgInfo("comment","end");
		return TOKEN_COMMENT_END;
};

"/**" {
	BEGIN _COMMENT_EX_START_;
	yylval.clear();
	csgInfo("commentEx","start");
	return TOKEN_COMMENT_EX_START;
};

<_COMMENT_EX_START_>({allletterEx}|{newline})*"*/" {
	BEGIN _COMMENT_EX_FILE_;
	csgInfo("commentEx","file");
	yylval.t_string+=yytext;
	csgInfo("commentExv",yytext);
	return TOKEN_COMMENT_EX_FILE;
};

<_COMMENT_EX_FILE_>{endline}+ {
	BEGIN INITIAL;
	csgInfo("commentEx","end");
	return TOKEN_COMMENT_EX_END;
};







{blank}	{
	csgInfo("blank","");
	};
{newline} {
	csgInfo("newline","");
	};

.	{
		std::string msg(yytext);
		std::string msgEx="\033[31m assert(false)\033[0m,["+msg+"]";
		csgInfo("unkonw",msgEx);
		//assert(false);
	};

<<EOF>>	{
		csgInfo("EOF","read input file end");
		return 0;
	};


%%

int yywrap(void){
	csgInfo("all end","read file end");
	return 1;
}

/*


{letter} {
	std::string msg(yytext);
	csgInfo("letter",msg);
};


{identify} {
	std::string msg(yytext);
	csgInfo("identify",msg);
};


[\t] {
	csgInfo("table","");
	};

[\n] {
	csgInfo("newline","");

	};


{blank} {
	std::string msg(yytext);
	csgInfo("blank",msg);
};


.	{
	std::string msg(yytext);
	csgInfo("unknow",msg);
	};


*/

