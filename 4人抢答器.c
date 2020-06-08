#include <reg51.h>  
#define uchar unsigned char
#define uint unsigned int
sbit smg1=P0^0;		     //��������ܵ�һλ
sbit k1=P2^3;			 //ѡ��1
sbit k2=P2^4;			 //ѡ��2
sbit k3=P2^5;			 //ѡ��3
sbit k4=P2^6;			 //ѡ��4
sbit keyks=P2^2;		  //���忪ʼ����
sbit keytz=P3^0;
sbit spk=P2^7;			 //���������
sbit led1=P3^6;
sbit led2=P3^7;
int djs,js;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,
                    0x92,0x82,0xf8,0x80,0x90};

uchar z;

void delayms(uint x)	  //��ʱ����
{  
    uint m,n;
for(m=x;m>0;m--)
  for(n=110;n>0;n--); 
}

void time1_init()		  //��ʱ��1��ʼ��
{
  TMOD=0x10;
  TR1=1;                  //��ʱ��1��������ֹͣ�ı�־λ
  ET1=1;				  //����ʱ��1�ж�
  EA=1;					  //�ж�������λ
  TH1=(65536-50000)/256;
  TL1=(65536-50000)%256;
}

void time1() interrupt 3   //��ʱ���ж�1
{
  TH1=(65536-50000)/256;
  TL1=(65536-50000)%256;
  js++;
  if(js>=20)
  {
   js=0;
   djs--;
  }
}

/**********��ʾ����**********/			//P1�Ƕοأ�P0��λ��
void djsxs()	  //����ʱ��ʾ
{ 
 int a,b;
 a=djs/10;		  //����ʱʮλ
 b=djs%10;		  //����ʱ��λ
 P1=table[a];
 P0=0X04;
 delayms(3);
 P0=0X00;

 P1=table[b];
 P0=0X08;
 delayms(3);
 P0=0X00;
}
void ksxs()		   //��ʼ����״̬��ʾ
{
 P1=0X0c;		   //��ʾP. P.P.
 P0=0X0d;
 delayms(3);
 P0=0X00;
}
void xs(uchar i)   //ѡ�ֺ���ʾ
{
 P1=table[i];
 smg1=1;
 delayms(3);
 smg1=0;
}
		
void djsjs()
{
 while(djs==0) 
	{spk=1;
	 led1=0;
	 led2=1;
	 TR1=0;
     djs=0;
	 djsxs();
	}
}
/**********������**********/
void main()
{ 
 time1_init();
 while(1)
 {
 spk=0;
 ksxs();
 TR1=0;
 if(keyks==0)
 {
  delayms(30);
  if(keyks==0)
  {
	while(!keyks);     //�����˰�������ʼ����
	djs=30;
	js=0;
	TR1=1;
	while(1)
	{
	 djsxs();
	 if(keyks==0)	   //��ʼ����
	 {
	  delayms(30);
	  if(keyks==0)
	  {
	   while(!keyks);
	   TR1=1;
	  }
	 }
	 if(keytz==0)	   //ֹͣ����
	 {
	  delayms(30);
	  if(keytz==0)
	  {
	   while(!keytz);
	   TR1=0;
	  }
	 }

	 if((k1==0||k2==0||k3==0||k4==0)&&(TR1==1))	 //�ж�ѡ�ְ���
	 { 
	  delayms(30);
	  if(k1==0||k2==0||k3==0||k4==0)
	  {
	   if(k1==0)z=1;
	   if(k2==0)z=2;
	   if(k3==0)z=3;
	   if(k4==0)z=4;
	   while(!k1||!k2||!k3||!k4);                //�ȴ������ɿ�
	   TR1=0;
	   led2=0;
	   while(1)
	   { 
	   djs=15;
	   js=0;
	   while(djs<=15)
	   {
		TR1=1;
		djsxs();
		xs(z);
		djsjs();
		}
		}
	   }
	  }
	   djsjs();
	 }
	}
  }
 }
 }


