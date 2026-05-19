#include <stdio.h>



char *buy_scene()
{
    int sceneNumber=0;


    int a,b;
    printf("スタート画面\n");
    printf("========================= メニュー =========================\n");
    printf("\n\n\n\n\n");





    printf("[1]読み込み画面へ\t\t[0]管理者画面へ");
    printf("\n");
    printf("=============================================================\n");

    printf("番号を入力\t:\t");
    scanf("%d",&sceneNumber);


  
    
    sum(a,b);

    if(sceneNumber==1)
    {
        return "Buy_Check";//ここでシーンの名前を入れる
    }
    else if(sceneNumber==0)
    {
        return "";
    }
    else
    {
        printf("無効な選択です\n");
    }
  


    return "Start";//シーンの名前を返す

}

float sum(float a, float b) {
a = 0;
b = 0;
return a + b;
}



float total()
{
    return sum(1,2);
}