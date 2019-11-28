#include "temClass.h"

//#pragma comment(lib, "json_vc71_libmtd.lib")

string getValue(string type, Json::Value value) {
	char a[20];
	if (type == "string")
		return "\"" + value.asString() + "\"";
	else if (type == "int")
		sprintf_s(a, "%d", value.asInt());
	else if (type == "double")
		sprintf_s(a, "%f", value.asDouble());
	else if (type == "bool")
		return value.asString();
	else 
		return "NULL";
	return string(a);
}

int main()
{
	Json::Value root;
	TemClass tem;
	ifstream is("model.json", ios::binary);

	if (!is.is_open())
	{
		cout << "open json file failed." << endl;
		return -1;
	}

	Json::Reader reader;	

	list<string*>  attrList;
	list<string*>  funcList;
	list<string*>  equList;
	list<string*>  varList;
	list<string*>  outList;
	//list<string*>  paraList;
	int cnt_double=0;
	int cnt_bool = 0;
	// 解析json内容
	if (reader.parse(is, root))
	{
		tem.setClassName(root["className"].asString());
		string a = getValue("int", Json::Value(root["stateNum"]));
		tem.setStateNum(a);
		a = getValue("int", Json::Value(root["doubleNum"]));
		tem.setDoubleNum(a);
		for (int i = 0; i < (int)root["attr"].size(); i++) {
			string *attr = new string[3];
			attr[0] = root["attr"][i]["type"].asString();
			attr[1] = root["attr"][i]["name"].asString();			
			attr[2] = getValue(attr[0], Json::Value(root["attr"][i]["value"]));
			attrList.push_back(attr);
			cout << attr[0] << " " << attr[1] << endl;
			if (attr[0] == "double") {
				cnt_double++;
			}
			if (attr[0] == "bool") {
				cnt_bool++;
			}
		}
		for (int i = 0; i < (int)root["func"].size(); i++) {
			string *func = new string[3];
			func[0] = root["func"][i]["type"].asString();
			func[1] = root["func"][i]["name"].asString();
			func[2] = root["func"][i]["value"].asString();
			cout << func[0] << " " << func[1] << func[2] << endl;
			funcList.push_back(func);
		}
		for (int i = 0; i < (int)root["equition"].size(); i++) {
			string *equ = new string[3];
			equ[0] = root["equition"][i]["type"].asString();
			equ[1] = root["equition"][i]["name"].asString();
			equ[2] = root["equition"][i]["value"].asString();
			cout << equ[0] << "(" << equ[1] <<") = " << equ[2] << endl;
			equList.push_back(equ);
		}
		
		for (int i = 0; i < (int)root["input"].size(); i++) {
			string a = root["input"][i]["type"].asString();			
			for (int j = 0; j < (int)root["input"][i]["vars"].size(); j++) {
				string *vars = new string[3];
				vars[0] = root["input"][i]["vars"][j]["name"].asString();
				vars[1] = root["input"][i]["vars"][j]["order"].asString();
				vars[2] = root["input"][i]["vars"][j]["type"].asString();
				varList.push_back(vars);
			}
			tem.add_ext_content(a,varList);
			for (int i = 0; i < (int)varList.size();) {
				varList.pop_back();
			}
		}
		for (int i = 0; i < (int)root["output"].size(); i++) {
			string portId = root["output"][i]["portId"].asString();
			for (int j = 0; j < (int)root["output"][i]["port"].size(); j++) {
				string type = root["output"][i]["port"][j]["type"].asString();
				for (int k = 0; k < (int)root["output"][i]["port"][j]["data"].size(); k++) {
					string *vars = new string[3];
					vars[0] = root["output"][i]["port"][j]["data"][k]["var"].asString();
					vars[1] = root["output"][i]["port"][j]["data"][k]["order"].asString();
					vars[2] = root["output"][i]["port"][j]["data"][k]["dataType"].asString();
					outList.push_back(vars);
				}
				tem.add_output_content(portId, type, outList);
				for (int i = 0; i < (int)outList.size();) {
					outList.pop_back();
				}
			}
			
			
		}
		for (int i = 0; i < (int)root["simPara"].size(); i++) {
			string *para = new string[4];
			para[0] = root["simPara"][i]["intError"].asString();
			para[1] = root["simPara"][i]["intMethod"].asString();
			para[2] = root["simPara"][i]["stepSize"].asString();
			para[3] = root["simPara"][i]["zeroPointError"].asString();
			tem.setOutFile(root["simPara"][i]["fileTail"].asString());
			tem.createMainFile(para);
		}
		tem.createHeaderFile(attrList, funcList, equList);
	}
	is.close();
	for (int i = 0; i < (int)attrList.size();) {
		attrList.pop_back();
	}
	for (int i = 0; i < (int)funcList.size();) {
		funcList.pop_back();
	}
	for (int i = 0; i < (int)funcList.size();) {
		equList.pop_back();
	}
	return 0;
}