#include <string.h>
#include <stdio.h>

char *age_Chack_Scene()
{
int sceneNumber=0;//ここはシーンが何番に移行するのかを記憶する変数
int age=0;
printf("========================= メニュー =========================\n");
printf("\n\n\n\n\n");

printf("年齢確認画面\n");

printf("お酒・タバコが含まれています。\n");

printf("1:はい(18歳以上) 2:いいえ(18歳未満)\n");

printf("\n");
printf("=============================================================\n");

printf("番号を入力\t:\t");

if(scanf("%d",&age)==1)
{
if(age==1)
{
printf("年齢確認が完了しました。\n");

}
else if(age==2)
{
printf("年齢確認ができませんでした。\n");
}
else
{
printf("無効な選択です\n");
}
}
else
{
printf("無効な入力です\n");
}



scanf("%d",&sceneNumber);



if(sceneNumber==1)
{
return "Buy_Chack";//ここでシーンの名前を入れる
}
else if(sceneNumber==0)
{
return "";
}else{
printf("無効な選択です\n");
return "";}









return "Start";//シーンの名前を返す
}