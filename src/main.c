#include <stdio.h>
#include <string.h>

#include"buy_Chack.c"
#include"age_Chack.c"
#include"chash_Chack.c"
#include"chash_select.c"
#include "point.c"
#include"pos_Code.c"
#include"start.c"
#include"admin.c"
#include"CSV.c"
int main(void) {
   char ret[64]={};//シーンの名前の受け渡しに使う変数
   strcpy(ret, "Start");//シーンの名前を受け取る


   while (1)
   {
    /* code */
   
   
    if(strcmp(ret,"Start")==0)
    {
      
        printf("\n\nStartに移行します\n");
        start_Scene();
        strcpy(ret, start_Scene());
    }
  
//    else if(strcmp(ret,"Buy_Check")==0)
//    {
//        printf("\n\nBuy_Checkに移行します\n");
//          buy_scene();
//          strcpy(ret, buy_scene());
       
//    }
//       else if(strcmp(ret,"admin")==0)
//    {
//        printf("\n\n管理者画面に移行します\n");
//          admin_Scene();
//          strcpy(ret, admin_Scene());
       
//    }
//    else if(strcmp(ret,"age_Check")==0)
//     {
//         printf("\n\nage_Checkに移行します\n");
//         age_Chack_Scene();
//         strcpy(ret, age_Chack_Scene());
//     }
//     else if(strcmp(ret,"chash_Check")==0)
//     {
//          printf("\n\nchash_Checkに移行します\n");
//          chash_Chack_Scene();
//          strcpy(ret, chash_Chack_Scene());
//     }
//     else if(strcmp(ret,"chash_Select")==0)
//     {
//          printf("\n\nchash_Selectに移行します\n");
//          chash_Select_Scene();
//          strcpy(ret, chash_Select_Scene());
//     }
//     else if(strcmp(ret,"point")==0)
//     {
//          printf("\n\npointに移行します\n");
//          point_Scene();
//          strcpy(ret, point_Scene());    
//     }
     else if(strcmp(ret,"pos_Code")==0)
     {
          printf("\n\npos_Codeに移行します\n");
          pos_Code_Scene();
          strcpy(ret, pos_Code_Scene()); 
     }
         else if(strcmp(ret,"CSV")==0)
   {
       printf("\n\nCSVに移行します\n");
        CSV_Scene();
         strcpy(ret, CSV_Scene());
       
   }

   }


return 0;

}