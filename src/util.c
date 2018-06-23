#include <stdio.h>
#include <string.h>
#include "util.h"

void parsing(char *buf, char * pToken, char **pArray,const char * separator)
{
	int i=0;
	pToken=strtok(buf,separator);
	while(pToken !=NULL){
		pArray[i]=pToken;
		//  printf("%d  %s\n",i,pArray[i]);
		if(++i >=ARR_CNT)   break;
		pToken=strtok(NULL,separator);
	}
}

