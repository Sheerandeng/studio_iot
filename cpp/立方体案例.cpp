#include <iostream>
using namespace std;

/*
设计立方体类（Cube）
求立方体的面积（2*a*b + 2*a*c + 2*b*c）和体积（a*b*c）
分别用全局函数和成员函数判断两个立方体是否相等？
*/

class Cube
{
public:
	// 设置长宽高
	void setL(int l)
	{
		m_L = l;
	}
	void setW(int w)
        {
                m_W = w;
        }
	void setH(int h)
        {
                m_H = h;
        }

	// 获取长宽高
	int getL()
	{
		return m_L;
	}
	int getW()
        {
                return m_W;
        }
	int getH()
        {
                return m_H;
        }

	// 求立方体的面积
	void getCubeS()
	{
		int S = 2*m_L*m_W + 2*m_L*m_H + 2*m_W*m_H;
		cout << "立方体的面积为：" << S << endl;
	}

	// 求立方体的体积
	void getCubeV()
	{
		int V = m_L*m_W*m_H;
		cout << "立方体的体积为：" << V << endl; 
	}

	// 通过成员函数判断是否相等
	bool compareCubeByClass( Cube &cub )
	{
		bool ret = ( cub.getL() == m_L && cub.getW() == m_W && cub.getH() == m_H );
		return ret;
	}

private:
	int m_L; // 长
	int m_W; // 宽
	int m_H; // 高
};

// 全局函数判断 两个立方体是否相等
bool compareCube( Cube &cub1, Cube &cub2 )
{
        if( cub1.getL() == cub2.getL() && cub1.getW() == cub2.getW() && cub1.getH() == cub2.getH() )
        {
                return true;
        }
        return false;
}

void test01()
{
	Cube c1;
	
	c1.setL(10);
	c1.setW(10);
	c1.setH(10);

	c1.getCubeS(); // 600
	c1.getCubeV(); // 1000

	// 通过全局函数判断两个立方体是否相等
	Cube c2;
	
	c2.setL(10);
        c2.setW(10);
        c2.setH(10);
	
	bool ret = compareCube( c1, c2 );
	if( ret == true )
	{
		cout << "c1和c2两个立方体相等" << endl;
	}
	else
	{
		cout << "c1和c2两个立方体不相等" << endl;
	}

	// 通过成员函数判断两个立方体是否相等
	bool ret2 = c1.compareCubeByClass( c2 );
        if( ret2 == true )
        {
                cout << "通过成员函数：c1和c2两个立方体相等" << endl;
        }
        else
        {
                cout << "通过成员函数：c1和c2两个立方体不相等" << endl;
        }
}

int main()
{
	test01();
	return 0;
}
