#include <string.h>
#include <stdio.h>


char SceneName[64]={};//シーンの名前の受け渡しに使う変数


char *start_Scene()
{
    int sceneNumber=0;//ここはシーンが何番に移行するのかを記憶する変数
    //表示部分
    printf("========================= メニュー =========================\n");
    printf("\n\n\n\n\n");






    printf("\n");
    printf("=============================================================\n");

    printf("番号を入力\t:\t");
    scanf("%d",&sceneNumber);


    if(sceneNumber==1)
    {
        strcpy(SceneName, "Buy_Check");//ここでシーンの名前を入れる
    }
  


    return SceneName;//シーンの名前を返す
}

