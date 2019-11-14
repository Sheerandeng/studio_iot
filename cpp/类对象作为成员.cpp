#include <iostream>
using namespace std;

class Phone
{
public:
	Phone()
	{
		cout << "手机的默认构造函数调用" << endl;
	}
	
	Phone(string name)
	{
		cout << "手机的有参构造函数调用" << endl;
		m_PhoneName = name;
	}
	
	~Phone()
	{
		cout << "手机的析构函数调用" << endl;
	}
	
	string m_PhoneName;
};

class Game
{
public:
	Game()
        {
                cout << "Game的默认构造函数调用" << endl;
        }
        
	Game(string name)
	{
		cout << "Game的有参构造函数调用" << endl;
		m_GameName = name;
	}
	
	~Game()
        {
                cout << "Game的析构函数调用" << endl;
        }
	
	string m_GameName;
};

class Person
{
public:
	Person()
	{
		cout << "Person的默认构造函数调用" << endl;	
	}

	~Person()
        {
                cout << "Person的析构函数调用" << endl;
        }

	Person(string name, string phoneName, string gameName) : m_Name(name), m_Phone(phoneName), m_Game(gameName)
	{
		cout << "Person的有参构造函数调用" << endl;
		//m_Name = name;
	}

	void playGame()
	{	
		cout << m_Name << "玩" << m_Phone.m_PhoneName << "牌的" << m_Game.m_GameName << "游戏" << endl;
	}

	string	m_Name;  // 姓名
	Phone	m_Phone; // 手机
	Game	m_Game;  // 游戏
};

void test01()
{
	Person p("狗蛋","苹果","王者荣耀");
	//p.m_Phone.m_PhoneName = "三星";
	//p.m_Game.m_GameName = "斗地主";
	p.playGame();
}

int main()
{
	test01();
	return 0;
}
