#include "include/json/json.h"
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;
class TemClass
{
private:
	string className;
	string protocol;
	string gc_func;
	string head;
	string constructor;
	string destructor;
	string init_func;
	string der_func;
	string state_func;
	string time_func;
	string int_func;
	string out_func;
	string ext_func;
	string ext_content;
	string post_func;
	string con_func;
	string stateNum;
	string doubleNum;
	string defined_func;

	string out_content;
	string main_func;

	map<string, string> varOrder;// order of variables
	map<string, string> name_addr;

	string fileTail;

public:
	TemClass();
	void setClassName(string clsnm);
	void setStateNum(string stateNum);
	void setDoubleNum(string doubleNum);
	void setOutFile(string fileTail);

	void createHeaderFile(list<string*> attrList, list<string*> funcList, list<string*> equList);
	void createCppFile(list<string*> attrList);
	void createMainFile(string* paraList);

	void add_init_func(list<string*> equList);
	void add_der_func(list<string*> equList);
	void add_ext_content(string type, list<string*> varList);
	void add_output_content(string port, string type, list<string*> outList);
	void add_output_func();
	void add_gc_func();
	void add_con_func();
	void add_ext_func();
	void add_time_func();
	void add_int_func();
	void add_state_func();
	void add_post_func();
	void add_destructor();
	virtual ~TemClass();
};

