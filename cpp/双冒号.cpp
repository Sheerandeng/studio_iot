#include <iostream>
using namespace std;

int atk = 200;

void test01()
{
	int atk = 100;
	cout << "攻击力为：" << atk << endl;
	cout << "全局攻击力为：" << ::atk <<endl;
}

int main()
{
        test01();
	return 0;
}
