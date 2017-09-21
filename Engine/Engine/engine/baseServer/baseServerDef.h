#ifndef _ENGINE_BASE_SERVER_DEF_H_
#define _ENGINE_BASE_SERVER_DEF_H_

enum EBaseServerDef
{
	EBaseServerUpdateDt = 1 ,//时间更新服务
	EBaseServerLoggerWritter = 2 ,//log服务器
	EBaseServerSocketRecv=4,//网络IO接收
	EBaseServerSocketSend=8,//网络IO发送
	EBaseServerMainLogic=16,//逻辑主线程

	EBaseServerAll = EBaseServerUpdateDt | EBaseServerLoggerWritter | EBaseServerSocketRecv | EBaseServerSocketSend |EBaseServerMainLogic,  //所有服务器
};


const int CSERVER_UPDATE_DATE_AMOUNT = 1;  //时间更新服务器线程数量
const int CSERVER_LOG_WRITTER_AMOUNT = 1;  //log服务器线程数量
const int CSERVER_MAIN_LOGIC_AMOUNT = 1;//逻辑线程数量
const int CSERVER_SOCKET_SEND_AMOUNT = 1; //消息发送线程


#endif