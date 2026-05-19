char *start_Scene()
{

int sceneNumber=0;//ここはシーンが何番に移行するのかを記憶する変数//表示部分
printf("スタート画面\n");
printf("========================= メニュー =========================\n");
printf("\n\n\n\n\n");

printf("ご来店ありがとうございます。\n");

printf("1:お買い物を開始する\t\t0:管理者メニュー\n");
printf("\n");
printf("=============================================================\n");

printf("番号を入力\t:\t");
scanf("%d",&sceneNumber);





if(sceneNumber==1)
{
return "Buy_Chack";//ここでシーンの名前を入れる
}
else if(sceneNumber==0)
{
return "pos_Code";
}
else
{
printf("無効な選択です\n");}









return "Start";//シーンの名前を返す
}

