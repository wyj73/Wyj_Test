#include "ConSpac_Inter.h"

//函数声明
char PCIe_InterMess2(int address );//判断某个地址上是否有PCIe设备
char PCIe_LookUp(int address ,char reg, unsigned short lenth, unsigned char * buf);//查询指定长度的PCIe的配置空间的寄存器内容 （byte word dword)
void PCIe_InterMess1(void);//遍历pci配置空间并打印出来
void PCIe_Ls(void);// 显示所有PCIe设备的bus dev func

//主函数
int main(void)
{
	unsigned short in_bus,in_dev,in_func,in_reg,in_len;
	unsigned char buffer[256];
	int address,i;
	char in_stu;
	PCIe_Ls();
	printf("请输入要访问的设备号bus ,dev,func \n");
	while(1)
	{
		scanf("%hd %hd %hd",&in_bus,&in_dev,&in_func);
		if(in_bus<=256&&in_dev<=32 && in_func<=8)
		{
			address=BASEADDR|(in_bus<<16)|(in_dev<<11)|(in_func<<8);
			if(PCIe_InterMess2(address)==0)
	    {
	       break;  
	    }
	    else
	       printf("请重新输入\n");
		}
		else
			printf("输入的设备号错误，请重新输入\n");
	}
	printf("请输入要要访问的寄存器地址, 访问长度 (byte:1 word:2 dword:4) \n");
	scanf("%hd %hd",&in_reg,&in_len);
	if(PCIe_LookUp(address ,in_reg, in_len, buffer)>=in_len)
	{
		for(i=0;i<in_len;i++)
		{
		  printf("%02x",buffer[i]);
		}
		printf("\n");
	}
	
	
	return 0;
}
/**
* 判断某个地址上是否有PCIe设备
* 输入: Bus ,dev ,func合成的地址，Register,
*       访问长度: 1: 1byte 2:word 4:dword 
**/
char PCIe_InterMess2(int address )
{
	//判断总线是否有效
  char stu=iopl(3);
	int data;
	char ret=0;
	if(stu<0)
    {
		printf("io特权级别更改失败\n");
		ret=1;
	}
	outl(address,CONFIG_ADDR);
	data=inl(CONFIG_DATA);
  if(data==0xffffffff)
	{
	 printf("此func上无PCI设备\n");
	 ret=2;
  }
	iopl(0);
	return ret;
}

/**
* 查询指定长度的PCIe的配置空间的寄存器内容
* 返回:查询到的数据长度
* lenth:1:byte 2:word 4:dword
**/
char PCIe_LookUp(int address ,char reg, unsigned short lenth, unsigned char * buf)
{
 int reg1,i,data;
 char num=reg%4;
 unsigned short j=0; 
  if((reg+lenth)>256)
  {
   printf("访问地址越界\n");
   return 0;
  }
  reg1=reg-num;
  iopl(3);
	outl(address+reg1,CONFIG_ADDR);
	for(i=0;i<lenth;i++)
	{
		buf[j++]= inb(CONFIG_DATA+num++);
		if(num>=4)break;
	}
	num=0;
	if(j<lenth)
	{
		outl(address+reg1+4,CONFIG_ADDR);
		while(num<4)
		{
		 buf[j++]= inb(CONFIG_DATA+num);
		 num++;
		 if(j>=lenth)break;
		}
	}
 iopl(0);
 return j;
}

/**
* 采用io端口寄存器遍历pci配置空间并打印出来
**/
void PCIe_InterMess1(void)
{
	int address;
	short  bus,dev,func;
  short reg;
	int data; //从config_data读出来的数据
  iopl(3);//读之前改变当前进程io特权级别为3
	for(bus=0;bus<MAX_BUS;bus++)
	{
		for(dev=0;dev<MAX_DEV;dev++)
		{
			for(func=0;func<MAX_FUNC;func++)
			{
				address=BASEADDR|(bus<<16)|(dev<<11)|(func<<8);
				outl(address,CONFIG_ADDR);
				data=inl(CONFIG_DATA);
				if(data!=0xffffffff && data!=0)
				{
				 	printf("bus: %hd,dev: %hd ,func: %hd \n",bus,dev,func);
					for(reg=0;reg<64;reg++)
					{
					  if(reg%8==0)
					  printf("\n");
						address = BASEADDR|(bus<<16)|(dev<<11)|(func<<8)|(reg);
						outl(address,CONFIG_ADDR);
						data=inb(CONFIG_DATA);
						printf("%02x ",(unsigned char)(data));
                        printf("%02x ",(unsigned char)(data>>8));
                        printf("%02x ",(unsigned char)(data>>16));
						printf("%02x ",(unsigned char)(data>>24));

						
					}
					printf("\n");
				}
			}
		}
	}
	iopl(0);//读完io级别设置为0
	

}
/***
* 显示所有PCIe设备的bus dev func
**/
void PCIe_Ls(void)
{
	int address;
	short  bus,dev,func;
  short reg;
	int data; //从config_data读出来的数据
  iopl(3);//读之前改变当前进程io特权级别为3
  printf("当前系统所有的PCI设备号如下:\n");
	for(bus=0;bus<MAX_BUS;bus++)
	{
		for(dev=0;dev<MAX_DEV;dev++)
		{
			for(func=0;func<MAX_FUNC;func++)
			{
				address=BASEADDR|(bus<<16)|(dev<<11)|(func<<8);
				outl(address,CONFIG_ADDR);
				data=inl(CONFIG_DATA);
				if(data!=0xffffffff && data!=0)
				{
				 	printf("bus: %02x,dev: %02x,func: %02x \n",bus,dev,func);
				}
			}
			
		}
   }
   iopl(0);
   printf("\n");
   
}



