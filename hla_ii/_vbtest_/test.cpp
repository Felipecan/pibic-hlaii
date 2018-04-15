#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int func(multimap<string, string> t)
{
	
}

int main()
{
	multimap<string, string> teste;

    teste.insert(make_pair<string, string>("VirtualBus.dut", "data1"));
    teste.insert(make_pair<string, string>("VirtualBus.dut", "data2"));
    teste.insert(make_pair<string, string>("VirtualBus.dut", "data3"));

    teste.insert(make_pair<string, string>("VirtualBus.refmod", "data3"));
    teste.insert(make_pair<string, string>("VirtualBus.refmod", "data2"));
    teste.insert(make_pair<string, string>("VirtualBus.refmod", "data4"));
    teste.insert(make_pair<string, string>("VirtualBus.refmod", "data5"));

    std::pair < multimap<string, string>::iterator, multimap<string, string>::iterator > ret;

    multimap<string, string>::iterator it;
    ret = teste.equal_range("VirtualBus.refmod");
    cout << "Size " << ret.first << endl;

    // if(it != teste.end())
    //     cout << "Valor encontrado" << endl;        
    // else
    //     cout << "Valor nao encontrado" << endl;
        


	//func(vs);
}
