#ifndef _ENGINE_NET_PROTOCOL_PROTOCOL_DEF_H_
#define _ENGINE_NET_PROTOCOL_PROTOCOL_DEF_H_

#include "../../def/csg_def.h"

struct SProtocolHead
{
	uint_t msgSize; //message block size
};


static const size_t SIZE_OF_PROTOCOL_HEAD = sizeof(SProtocolHead);



#endif