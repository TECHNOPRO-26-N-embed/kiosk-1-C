#include <stdio.h>
#include <string.h>

#include"buy_Chack.c"
#include"age_Chack.c"
#include"chash_Chack.c"
#include"chash_select.c"
#include "point.c"
#include"pos_Code.c
#include"start.c"
char ret[64]={};//シーンの名前の受け渡しに使う変数

int main(void) {
    
   ret[64]=start_Scene();//シーンの名前を受け取る


   if(strcmp(ret,"Buy_Check")==0)
   {
       printf("Buy_Checkに移行します");
   }




return 0;

}