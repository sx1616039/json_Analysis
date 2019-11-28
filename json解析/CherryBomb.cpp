#include <string.h>
#include "CherryBomb.h"

CherryBomb::CherryBomb():ode_system<IO_Type>(3, 1){
	height = 1.000000;
	velocity = 0.000000;
	time = 0.000000;
	phase = 0;
	type = -1;
	isDropped = false;
}
void CherryBomb::init(double *q){
	q[0] = height;
	q[1] = velocity;
	q[2] = time;
}
void CherryBomb::der_func(const double* q, double* dq) {
	dq[0] = q[1];
	dq[1] = -9.8;
	dq[2] = 1;
}
double CherryBomb::time_event_func(const double* q){
}
void CherryBomb::state_event_func(const double* q, double *z) {
}
void CherryBomb::internal_event(double* q, const bool* state_event){
}
void CherryBomb::external_event(double* q, double e, const Bag<IO_Type>& xb){
	Bag<IO_Type>::const_iterator i = xb.begin();
	for (; i != xb.end(); i++){
		int type = (*i).value->type;
		char *data = (*i).value->msg;
		if (type==1){
			memcpy(&isDropped,data+4,sizeof(bool));
			memcpy(&phase,data+0,sizeof(int));
		}
		if (type==0){
			memcpy(&height,data+0,sizeof(double));
		}
	}

}
void CherryBomb::output_func(const double *q, const bool* state_event,Bag<IO_Type>& yb){
	if (type==0){
		Protocol *p = new Protocol();
		p->type = 0;
		p->srcId = -1;
		p->tarId = -1;
		p->time = 0;
		p->length = 16;
		p->msg = new char[16+1];
		memcpy(p->msg+8,&height,sizeof(double));
		memcpy(p->msg+0,&velocity,sizeof(double));
		p->msg[16]='\0';
		IO_Type y;
		y.port = 0;
		y.value = p;
		yb.insert(y);
	}
	if (type==1){
		Protocol *p = new Protocol();
		p->type = 1;
		p->srcId = -1;
		p->tarId = -1;
		p->time = 0;
		p->length = 8;
		p->msg = new char[8+1];
		memcpy(p->msg+0,&height,sizeof(double));
		p->msg[8]='\0';
		IO_Type y;
		y.port = 0;
		y.value = p;
		yb.insert(y);
	}

}
void CherryBomb::postStep(double* q){
}
void CherryBomb::confluent_event(double* q, const bool* state_event, const Bag<IO_Type>& xb){
	internal_event(q, state_event);
	external_event(q, 0.0, xb);
}
void CherryBomb::gc_output(Bag<IO_Type>&g){
	Bag<IO_Type>::iterator i;
	for (i = g.begin(); i != g.end(); i++)
	{
		delete (*i).value;
	}
}
int CherryBomb::getPhase()
{
	return phase;
}


