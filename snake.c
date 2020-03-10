#include <intrins.h>
#include <stdlib.h>
#include <reg52.h>

//IO配置
sbit LEDARRAY_D = P2^0;
sbit LEDARRAY_C = P2^1;
sbit LEDARRAY_B = P2^2;
sbit LEDARRAY_A = P2^3;
sbit LEDARRAY_G = P2^4;
sbit LEDARRAY_DI = P2^5;
sbit LEDARRAY_CLK = P2^6;
sbit LEDARRAY_LAT = P2^7;

sbit KEY_Right=P1^7;			//右
sbit KEY_Down=P1^6;			//下
sbit KEY_Left=P1^5;			//左
sbit KEY_Up=P1^4;			//上
sbit KEY_Star=P3^7;		//开始
sbit KEY_Restar=P3^2;		//重新开始
sbit KEY_Switch=P3^6;		//选择难度
#define Right 0
#define Left 2
#define Up 3
#define Down 1

//函数或者变量声明
 void Send( unsigned char dat);
 void Scan_Line( unsigned char m);
void ReInit();
void Display();
void Init();
void Shift();
void Random_Dot();
void Scan_Key();

void Delay_ms( unsigned int m);
void delay(int n);
unsigned char Display_Buffer[2];

unsigned char table[31]={0x00,0x10,0x20};
unsigned char count = 3;
unsigned char Direction;
unsigned char T;
int tt,n,z;
unsigned char Flag_Shift,Flag_Over,flag3,Flag_Dot,Flag_Draw;
unsigned char u;
//************************************************************
//主函数
//*************************************************************
void main()
{  
	Init();
	while(1)
	{
		if (Flag_Dot == 1)			//开启按键按下
			Random_Dot();			//随即显示一个点

		if (Flag_Shift == 1)		
		
		Shift();				//贪食蛇移动
		Scan_Key();			//扫描按键
		Display();			//显示
						if (z==0)		//是否按下暂停按键
				{
					while (1)
					{
						Display();
						if (KEY_Star == 0)						
						{
						break;
						}		
	
			      }		
				 }
	}
}


//**********************************************************
//延时函数
//**********************************************************
void Delay_ms( unsigned int m)
{
	unsigned int j;
	unsigned int i;
	for(i = 0 ; i < m ; i++)
	{
		for(j = 0 ; j < 123 ; j++);
	}
}


//**********************************************************
//延时函数
//**********************************************************
void delay(int n)
{
	while(n--);
}
//**********************************************************
//重新初始化
//**********************************************************
void ReInit()
{	
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	TR0=0;
	z=0;
	T=0;
	Direction=0;
	count=3;
	table[0]=0x00;
	table[1]=0x10;
	table[2]=0x20;
	Delay_ms(500);
	Flag_Over=0;
	Flag_Dot=2;
	Flag_Draw=1;
	table[count]=0;

}
//**********************************************************
//初始化
//**********************************************************
void Init()
{
	Flag_Draw=1;
	Flag_Dot=2;
	tt=0;
	T=0;
	z=0;
	count=0;
	Flag_Shift=0;
	Flag_Over=0;
	n=0;						//初始难度				
	EA=1;						//开总中断
	IT0=1;						//低电平触发
	ET0=1;						//定时器0中断使能
	ET1=1;						//定时器1中断使能
	TMOD=0x11;					//定时器0模式1，定时器1模式1
	TH0=(65535-50000)/256;		//定时器0设定初值
	TL0=(65535-50000)%256;
	TH1=(65535-10000)/256;		//定时器1设定初值
	TL1=(65535-10000)%256;
	Direction=0;						//方向
	TR0=0;						//定时器0不开启
	TR1=1;						//定时器1开启
}

//**********************************************************
//按键扫描
//**********************************************************
void Scan_Key()
{
	if(KEY_Switch == 0)
	{
			n=n+1;
			if (n>2)
			{ 
				n=n-3;
			}
			count=n;
			Delay_ms (500);
	}
	if(KEY_Star == 0)
		{	

			if (Flag_Dot==2)
			{	
				count=3;
				Flag_Dot=1;			//启动游戏
				TR0 = 1;			//开始移动
			}	
				z=z+1;
				z=z%2;
				Delay_ms (500);
		}

	if(KEY_Restar == 0)
	{
			ReInit();
	}

	if(KEY_Right == 0)
	{
		if(Direction != Left)
			Direction = Right;
	}

	if(KEY_Down == 0)
	{
		if(Direction != Up)
		Direction = Down;	
	}

	if(KEY_Left == 0)
	{
		if(Direction != Right)
		Direction = Left;
	
	}

	if(KEY_Up == 0)
	{
		if(Direction != Down)
		Direction = Up;	
	}
}

//**********************************************************
//
//**********************************************************
void Shift()
{
	int k;
	if ( Flag_Draw == 1 )
	{
		for(k=0;k<count-1;k++)				//绘制蛇身
		{
			table[k]=table[k+1];
		}
	}
	switch (Direction)
	{
		case 0:							//向右
			if (table[count-1]/16<15)				//蛇头没有超过右边框
				table[count-1]=table[count-1]+16;	//右移
			else
				Flag_Over=1;							//蛇头超过边框，游戏结束
			break;
		case 1:							//向下
			if (table[count-1]%16<15)
				table[count-1]=table[count-1]+1;
			else
				Flag_Over=1;
			break;
		case 2:							//向左
 			if (table[count-1]/16>0)
				table[count-1]=table[count-1]-16;
			else
				Flag_Over=1;
			break;
		case 3:							//向上
			if (table[count-1]%16>0)
				table[count-1]=table[count-1]-1;
			else
				Flag_Over=1;
			break;
		default:
			break;
	}
	Flag_Shift=0;
	if (Flag_Over == 0)
	{
		for (k=0;k<count-1;k++)							//判断是否蛇头碰撞到蛇身
		{
			if (table[count-1]==table[k])
			{
				Flag_Over=1;
				break;
			}
		}
	}
	if (Flag_Over == 1)
	{ 
		ReInit();					//重现开始游戏
	}
	else if (table[count-1]==table[count])		//蛇头 碰到 食物
	{
		if (count<30)							
		{
			count++;							//蛇身长度+1
			Flag_Draw=0;
		}
		Flag_Dot=1;
		table[count]=0xff;						//新的食物的坐标在没随机前，不显示
	}
	else
	{
		Flag_Draw=1;
	}		
}

//**********************************************************
//随机函数
//**********************************************************
void Random_Dot()
{
	int k;
	Flag_Dot=0;
	do
	{
		flag3=1;
		u=rand()%256;				//取出0-255中的任意一个值
		u=u/16*10+u%16;				//取出随即点的行号放高4位，列号放低4位
		for (k=0;k<count;k++)		//是否与蛇身重合
		{
			if (u==table[k])		//假如有重合，重新随即
			{
				flag3=0;
				break;
			}
		}
	}
	while(!flag3);					//假如flag3 == 1跳出循环，如果为0则重新随即点
	table[count]=u;					//取出随即点的值放在table[count]中
}

//**********************************************************
//定时器0中断
//**********************************************************
void time0() interrupt 1
{
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	T++;
	if (T>=3*n+4)
	{
		T=0;
		Flag_Shift=1;
	}
}

//**********************************************************
//定时器中断1
//**********************************************************
void time1() interrupt 3
{
	TH1=(65535-10000)/256;		//定时器1设定初值
	TL1=(65535-10000)%256;
	tt++;			  //获取随机数新种子
	srand(tt);
	if 	(tt>=2157)
	tt=0;

}


//************************************************************
//显示函数
//*************************************************************
void Display()					
{
	unsigned char i,j;
	unsigned int temp = 0x7fff;
	unsigned char x,y;

	for( j = 0 ; j <= count ; j++ )
	{
		LEDARRAY_G = 1;			//更新数据时候关闭显示。等更新完数据，打开138显示行。防止重影。

		y=table[j]/16;				//高4位放列值，  行值：0 = 选通  1 = 不选
		x=table[j]%16;				//低4位放行值，  列值：1 = 选通  0 = 不选
		y=15-y;

		temp = 0x7fff;		
		for(i = 0 ; i < y ; i++)
		{
			temp = (temp>>1)|0x8000;
		} 

		Display_Buffer[1] = temp&0x00ff;
		Display_Buffer[0] = (temp>>8)&0x00ff;
				
		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		LEDARRAY_LAT = 1;					//锁存数据
		_nop_();
	
		LEDARRAY_LAT = 0;
		_nop_();

		Scan_Line(x);						//选择第i行

		LEDARRAY_G = 0;
		
		delay(50);		//延时一段时间，让LED亮起来。				
	}	
}

//****************************************************
//扫描某一行
//****************************************************
void Scan_Line( unsigned char m)
{	
	switch(m)
	{
		case 0:			
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 0; 					
			break;
		case 1:					
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 0;		
			break;
		case 2:					
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 0; 		
			break;
		case 3:					
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 0; 		
			break;
		case 4:
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 0;		
			break;
		case 5:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 6:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 7:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 8:
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 9:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;	
		case 10:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 11:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 12:
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 13:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 14:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 15:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		default : break;	
	}
}

//****************************************************
//发送数据
//****************************************************
void Send( unsigned char dat)
{
	unsigned char i;
	char j = 0;
	LEDARRAY_CLK = 0;
	_nop_();	
	LEDARRAY_LAT = 0;
	_nop_();

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 )
		{
			LEDARRAY_DI = 1;	
		}
		else
		{
			LEDARRAY_DI = 0;
		}


		LEDARRAY_CLK = 1;				//上升沿发送数据
			_nop_();
		LEDARRAY_CLK = 0;
			_nop_();

		
		dat >>= 1;			
	}			
}
