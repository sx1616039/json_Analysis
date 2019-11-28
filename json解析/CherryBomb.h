#pragma once
#include "adevs.h"
#include "protocol.h"
using namespace adevs;

class CherryBomb :public ode_system<IO_Type> {
public:
	CherryBomb();
	void init(double *q);
	void der_func(const double* q, double* dq);
	void state_event_func(const double* q, double *z);
	double time_event_func(const double* q);
	void external_event(double* q, double e, const Bag<IO_Type>& xb);
	void internal_event(double* q, const bool* state_event);
	void confluent_event(double* q, const bool* state_event, const Bag<IO_Type>& xb);
	void output_func(const double *q, const bool* state_event,Bag<IO_Type>& yb);
	void postStep(double* q);
	void gc_output(Bag<IO_Type>&g);
private:
	double height;
	double velocity;
	double time;
	int phase;
	int type;
	bool isDropped;
	
public:
	int getPhase();
	
};
