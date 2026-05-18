#include <string.h>
#include <stdio.h>


int sceneNumber=0;//ここはシーンが何番に移行するのかを記憶する変数
char SceneName[]={};//シーンの名前の受け渡しに使う変数

char start_Scene()
{//表示部分
    printf("========================= メニュー =========================\n");
printf("\n\n\n\n\n");






printf("\n");
printf("=============================================================\n");

printf("番号を入力\t:\t");
scanf("%d",&sceneNumber);


if(sceneNumber==1)
{
    SceneName[64]="Buy_Check";//ここでシーンの名前を入れる

 
}
else if(sceneNumber==0)
{

}


return SceneName;//シーンの名前を返す
}
