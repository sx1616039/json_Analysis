#include "TemClass.h"


int getLength(string type) {
	if (type == "int")
		return sizeof(int);
	else if (type == "bool")
		return sizeof(bool);
	else if (type == "double")
		return sizeof(double);
	else {
		cout << "unknown data type!" << endl;
		return -1;
	}
}
map<string, string> getStartAddr(list<string*> varList, string &totalLength) {
	int totalLen = 0;
	map <string, string> order_name;
	map <string, int> order_len;
	map <string, string> name_addr;
	list<string*>::iterator it;
	for (it = varList.begin(); it != varList.end(); it++) {
		string a[3];
		a[0] = (*it)[0];
		a[1] = (*it)[1];
		a[2] = (*it)[2];
		order_name[a[1]] = a[0];
		order_len[a[1]] = getLength(a[2]);
		totalLen += order_len[a[1]];
	}
	char total[10];
	sprintf_s(total, "%d", totalLen);
	totalLength = string(total);
	for (int k = 0; k < (int)order_name.size(); k++) {
		char sk[10], length[10], ord[10];
		sprintf_s(sk, "%d", k);
		int len = 0;
		for (int n = 0; n < k; n++) {
			sprintf_s(ord, "%d", n);
			len += order_len[ord];
		}
		sprintf_s(length, "%d", len);
		name_addr[order_name[string(sk)]] = string(length);
	}
	return name_addr;
}
TemClass::TemClass()
{
	className = "";
	protocol="";
	gc_func = "";
	head = "";
	constructor = "";
	init_func = "";
	der_func = "";
    state_func = "";
	time_func = "";
	int_func = "";
	out_func = "";
	ext_func = "";
	post_func = "";
	con_func = "";
	stateNum = "";
	doubleNum = "";
	defined_func = "";
	ext_content = "";
	out_content = "";
	main_func = "";
	fileTail = "";
	destructor = "";
}

void TemClass::createHeaderFile(list<string*> attrList, list<string*> funcList, list<string*> equList) {
	ofstream out;	
	string fileName = "C:\\Users\\wxq\\Desktop\\modelTransform\\"+className + ".h";
	out.open(fileName.c_str());	
	string head = "";
	head += "#pragma once\n#include \"adevs.h\"\n";
	head += "#include <fstream>\n";
	head += "#include \"protocol.h\"\n";
	head += "using namespace adevs;\n\nclass "
		+ className + " :public ode_system<IO_Type> {\n"
		+ "public:\n\t"
		+ className + "();\n\t"
		+ "void init(double *q);\n\t"
		+ "void der_func(const double* q, double* dq);\n\t"
		+ "void state_event_func(const double* q, double *z);\n\t"
		+ "double time_event_func(const double* q);\n\t"
		+ "void external_event(double* q, double e, const Bag<IO_Type>& xb);\n\t"
		+ "void internal_event(double* q, const bool* state_event);\n\t"
		+ "void confluent_event(double* q, const bool* state_event, const Bag<IO_Type>& xb);\n\t"
		+ "void output_func(const double *q, const bool* state_event,Bag<IO_Type>& yb);\n\t"
		+ "void postStep(double* q);\n\t"
		+ "void gc_output(Bag<IO_Type>&g);\n";
	head += "~" + className + "();\n";
	head += "private:\n\t";
	list<string*>::iterator it;
	for (it = attrList.begin(); it != attrList.end(); it++) {
		string a[3];
		a[0] = (*it)[0];
		a[1] = (*it)[1];
		a[2] = (*it)[2];
		head += a[0] + " " + a[1] + ";\n\t";
	}
	head += "\npublic:\n\t";
	for (it = funcList.begin(); it != funcList.end(); it++) {
		string a[3];
		a[0] = (*it)[0];
		a[1] = (*it)[1];
		a[2] = (*it)[2];
		defined_func += a[0] + " " + className + "::" + a[1] + "\n{\n\t"+a[2] + "\n}\n\n";
		head += a[0] + " " + a[1] + ";\n\t";
	}
	head += "\n};\n";
	out << head;
	out.close();

	ofstream outcpp; 
	fileName = "C:\\Users\\wxq\\Desktop\\modelTransform\\" + className + ".cpp";
	outcpp.open(fileName.c_str());
	createCppFile(attrList);
	add_init_func(equList);
	add_der_func(equList);
	add_gc_func();
	add_con_func();
	add_ext_func();
	add_output_func();
	add_time_func();
	add_int_func();
	add_state_func();
	add_post_func();
	add_destructor();
	outcpp << constructor << init_func << der_func << time_func <<state_func<< int_func<<destructor
		<< ext_func	<< out_func << post_func << con_func << gc_func << defined_func << endl;
	
	outcpp.close();
}
void TemClass::add_state_func() {
	state_func += "void " + className + "::state_event_func(const double* q, double *z) {\n";
	state_func += "\tif (q[" + varOrder["velocity"] + "] < 0.0) z[0] = q[" + varOrder["height"] + "];\n";
	state_func += "\telse z[0] = 1;\n";
	state_func += "}\n";
}
void TemClass::add_int_func() {
	int_func += "void " + className + "::internal_event(double* q, const bool* state_event){\n";
	int_func += "if (state_event[0]) q[" + varOrder["velocity"] + "] = -q[" + varOrder["velocity"] + "];\n";
	int_func += "if (state_event[1]) phase = 1;\n";
	int_func += "}\n";

}
void TemClass::add_time_func() {
	time_func += "double " + className + "::time_event_func(const double* q){\n\t";
	time_func += "if (nextTime > q[" + varOrder["time"] + "])\n\t\t";
	time_func += "return nextTime - q[" + varOrder["time"] + "];\n\t";
	time_func += "else \n\t\t";
	time_func += "return DBL_MAX;\n}\n";
}
void TemClass::createMainFile(string* paraList) {
	ofstream out_main;
	out_main.open("C:\\Users\\wxq\\Desktop\\testGenCode\\testGenCode\\main.cpp");
	if (!out_main) {
		cout << "error!" << endl;
	}
	main_func += "#include \"" + className + ".h\"\n";
	main_func += "#include \"protocol.h\"\n";
	main_func += "#include <iostream>\n";
	main_func += "using namespace std;\n\n";
	main_func += "int main(){\n\t";
	main_func += className+"* bomb = new "+ className+"();\n\t";
	main_func += "ode_solver<IO_Type>* ode_solve =\n\t";
	main_func += "\t new "+ paraList[1]+"<IO_Type>(bomb, "+ paraList[0]+","+ paraList[2]+");\n";
	main_func += "\tevent_locator<IO_Type>* event_find =\n\t";
	main_func += "\tnew linear_event_locator<IO_Type>(bomb, " + paraList[3] + ");\n";
	main_func += "\tHybrid<IO_Type>* model = new Hybrid<IO_Type>(bomb, ode_solve, event_find);\n";
	main_func += "\tSimulator<IO_Type>* sim = new Simulator<IO_Type>(model);\n";
	main_func += "\twhile (bomb->getPhase() == 0)\n";
	main_func += "\t\tsim->execNextEvent();\n";
	main_func += "\tdelete sim; delete bomb;\n";
	main_func += "\treturn 0;\n}";
	out_main << main_func;
	out_main.close();
}
void TemClass::add_output_content(string port, string type, list<string*> outList) {
	string t = "";
	string len;
	map<string, string> name_addr = getStartAddr(outList, len);
	t += "\tif (type==" + type + "){\n\t";
	t += "\tProtocol *p = new Protocol();\n\t";
	t += "\tp->type = "+type+";\n\t";
	t += "\tp->srcId = -1;\n\t";
	t += "\tp->tarId = -1;\n\t";
	t += "\tp->time = 0;\n\t";
	t += "\tp->length = "+len+";\n\t";
	t += "\tp->msg = new char[" + len + "+1];\n\t";
	list<string*>::iterator it;
	for (it = outList.begin(); it != outList.end(); it++) {
		string a[3];
		a[0] = (*it)[0];
		a[1] = (*it)[1];
		a[2] = (*it)[2];
		t += "\tmemcpy(p->msg+" + name_addr[a[0]]+",&" + a[0] + "," + "sizeof(" + a[2] + "));\n\t";
	}
	t += "\tp->msg[" + len + "]='\\0';\n\t";
	t += "\tIO_Type y;\n\t";
	t += "\ty.port = " + port + ";\n\t";
	t += "\ty.value = p;\n\t";
	t += "\tyb.insert(y);\n\t}\n";

	out_content += t;
}
void TemClass::add_output_func() {
	out_func += "void " + className + "::output_func(const double *q, const bool* state_event,Bag<IO_Type>& yb){\n";
	out_func += out_content;
	out_func += "\n}\n";
}
void TemClass::createCppFile(list<string*> attrList) {
	constructor += "#include <string.h>\n";
	constructor += "#include \"" + className + ".h\"\n\n";
	constructor += className + "::" + className + "():ode_system<IO_Type>(";
	constructor += doubleNum + ", " + stateNum;
	constructor += "){\n";
	list<string*>::iterator it;
	for (it = attrList.begin(); it != attrList.end(); it++) {
		string a[3];
		a[0] = (*it)[0];
		a[1] = (*it)[1];
		a[2] = (*it)[2];
		if (a[2]!="NULL")
			constructor += "\t" + a[1] + " = " + a[2] + ";\n";
	}
	constructor += "\toutFile.open(\"data" + fileTail + "\");\n";
	constructor += "\tif (!outFile) cout<<\"open file error!\"<<endl;\n";
	constructor += "}\n";
}

void TemClass::add_init_func(list<string*> equList) {
	init_func += "void " + className + "::init(double *q){";
	int k = 0;
	list<string*>::iterator it;
	for (it = equList.begin(); it != equList.end(); it++) {
		string a = (*it)[1];
		if (a != "time") {
			char sk[10];
			sprintf_s(sk, "%d", k);
			varOrder[a] = string(sk);
			init_func += "\n\tq[" + string(sk);
			init_func += "] = " + a + ";";
			k++;
		}
	}
	// Ä¬ÈÏtimeÅÅ×îºó
	char sk[10];
	sprintf_s(sk, "%d", k);
	varOrder["time"] = string(sk);
	init_func += "\n\tq[" + string(sk) + "] = time;";
	init_func += "\n}\n";
}
void TemClass::add_der_func(list<string*> equList) {
	der_func += "void " + className + "::der_func(const double* q, double* dq) {";
	list<string*>::iterator it;
	for (it = equList.begin(); it != equList.end(); it++) {
		string a[3];
		a[0] = (*it)[0];
		a[1] = (*it)[1];
		a[2] = (*it)[2];
		if (a[1] != "time") {
			if (a[0] == "var") {
				der_func += "\n\tdq[" + varOrder[a[1]] + "] = q[" + varOrder[a[2]] + "];";
			}
			else {
				der_func += "\n\tdq[" + varOrder[a[1]] + "] = " + a[2] + ";";
			}
		}
		else {
			der_func += "\n\tdq[" + varOrder["time"] + "] = 1;";
		}
	}
	der_func += "\n}\n";
}

void TemClass::add_ext_content(string type, list<string*> varList) {
	string len;
	name_addr = getStartAddr(varList,len);
	ext_content += "\tif (type==" + type + "){\n\t\t";
	list<string*>::iterator it;
	for (it = varList.begin(); it != varList.end(); it++) {
		string a[3];
		a[0] = (*it)[0];
		a[1] = (*it)[1];
		a[2] = (*it)[2];
		ext_content += "\tmemcpy(&" + a[0] + ",data+" + name_addr[a[0]] + "," + "sizeof(" + a[2] + "));\n\t\t";
	}
	ext_content += "}\n\t";
}

void TemClass::add_con_func() {
	con_func += "void " + className + "::confluent_event(double* q, const bool* state_event, const Bag<IO_Type>& xb){\n";
	con_func += "\tinternal_event(q, state_event);\n";
	con_func += "\texternal_event(q, 0.0, xb);";
	con_func += "\n}\n";
}
void TemClass::add_ext_func() {
	ext_func += "void " + className + "::external_event(double* q, double e, const Bag<IO_Type>& xb){\n\t";
	ext_func += "Bag<IO_Type>::const_iterator i = xb.begin();\n\t";
	ext_func += "for (; i != xb.end(); i++){\n\t\t";
	ext_func += "int type = (*i).value->type;\n\t\t";
	ext_func += "char *data = (*i).value->msg;\n\t";
	ext_func += ext_content;
	ext_func += "}\n";
	ext_func += "\n}\n";
}

void TemClass::add_gc_func() {
	gc_func += "void " + className + "::gc_output(Bag<IO_Type>&g){";
	gc_func += "\n\tBag<IO_Type>::iterator i;\n\t";
	gc_func += "for (i = g.begin(); i != g.end(); i++)\n\t";
	gc_func += "{\n\t\t";
	gc_func += "delete (*i).value;\n\t}\n";
	gc_func += "}\n";
}
TemClass::~TemClass()
{
}
void TemClass::add_destructor() {
	destructor += className + "::~" + className + "(){\n";
	destructor += "\toutFile.close();\n";
	destructor += "}\n";
}
void TemClass::setClassName(string clsnm) {
	className = clsnm;
}
void TemClass::setStateNum(string stateNum) {
	this->stateNum = stateNum;
}
void TemClass::setDoubleNum(string doubleNum) {
	this->doubleNum = doubleNum;
}
void TemClass::setOutFile(string fileTail) {
	this->fileTail = fileTail;
}
void TemClass::add_post_func() {
	post_func += "void " + className + "::postStep(double* q){\n";
	post_func += "\toutFile << q[0] <<\",\"<< q[1] <<\",\"<< q[2]<<endl;\n";
	post_func += "}\n";
}