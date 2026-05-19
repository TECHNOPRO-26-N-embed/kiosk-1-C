#include <stdio.h>
#include <string.h>

#include"buy_Chack.c"
#include"age_Chack.c"
#include"chash_Chack.c"
#include"chash_select.c"
#include "point.c"
#include"pos_Code.c"
#include"start.c"


int main(void) {
   char ret[64]={};//シーンの名前の受け渡しに使う変数
   strcpy(ret, start_Scene());//シーンの名前を受け取る

    if(strcmp(ret,"Start")==0)
    {
      
        printf("Startに移行します\n");
        start_Scene();
        strcpy(ret, start_Scene());
    }
   else if(strcmp(ret,"Buy_Check")==0)
   {
       printf("Buy_Checkに移行します\n");
         buy_scene();
         strcpy(ret, buy_scene());
       
   }
   else if(strcmp(ret,"age_Check")==0)
    {
        printf("age_Checkに移行します\n");
        age_Chack_Scene();
        strcpy(ret, age_Chack_Scene());
    }
    else if(strcmp(ret,"chash_Check")==0)
    {
         printf("chash_Checkに移行します\n");
         chash_Chack_Scene();
         strcpy(ret, chash_Chack_Scene());
    }
    else if(strcmp(ret,"chash_Select")==0)
    {
         printf("chash_Selectに移行します\n");
         chash_Select_Scene();
         strcpy(ret, chash_Select_Scene());
    }
    else if(strcmp(ret,"point")==0)
    {
         printf("pointに移行します\n");
         point_Scene();
         strcpy(ret, point_Scene());    
    }
    else if(strcmp(ret,"pos_Code")==0)
    {
         printf("pos_Codeに移行します\n");
         pos_Code_Scene();
         strcpy(ret, pos_Code_Scene()); 
    }




return 0;

}