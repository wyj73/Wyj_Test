#ifndef _CONSPAC_INTER_H
#define _CONSPAC_INTER_H

#include <stdio.h>
#include <string.h>
#include <sys/io.h>
#include <stdlib.h>
#include <unistd.h>

//PCI设备bus,Dev,func最大值
//2021年11月9日
#define MAX_BUS  256
#define MAX_DEV  32
#define MAX_FUNC 8

#define CONFIG_ADDR 0xcf8
#define CONFIG_DATA 0xCFC
#define BASEADDR 0x80000000


#endif
