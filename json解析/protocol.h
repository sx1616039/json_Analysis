#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "adevs.h"


struct Protocol
{
	int         srcId;
	int         tarId;
	int         length;
	int         type;
	double      time;
	char*       msg;
};

typedef adevs::PortValue<Protocol*> IO_Type;


#endif //PROTOCOL_H
