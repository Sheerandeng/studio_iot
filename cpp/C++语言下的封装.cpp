#include <iostream>
#include <cstring>
using namespace std;

struct Person
{
	char mName[64];
	
	int mAge;

	void PersonEat()
	{
		cout << mName << "吃人饭" << endl;
	}
};

struct Dog
{
        char mName[64];

        int mAge;

        void DogEat()
        {
                cout << mName << "吃狗粮" << endl;
        }
};

// C++中的封装 严格类型转换检测 让属性和行为绑定在一起
// 属性和行为作为一个整体来表示生活中的事物
// 控制权限 pubilc 公有权限 protected 保护权限 privete 私有权限
void test01()
{
	Person p1;
	strcpy(p1.mName,"老王");
	p1.PersonEat();
}

// struct 和 class 是一个意思，唯一的不同是默认权限
// struct 是 public，但是 class 是 private
// 所谓的私有权限 就是私有成员（属性、函数） 在类的内部可以访问，类外部不可以访问
// 公有权限 在类的内部和外部都可以访问

class Animal
{
private:
	void eat()
	{
		mAge = 100;
	}

	int mAge;
public:
	int mHeight;
protected: //保护权限，类内部可以访问，外部不可以访问，当前类的子类可以访问
	int mWeigt;
};

void test02()
{
	Animal an;
	// an.eat();
	// an.mAge; // 私有不可以访问

	an.mHeight = 100; // 公有权限可以在类的外部访问
}

int main()
{
	test01();
	return 0;
}
