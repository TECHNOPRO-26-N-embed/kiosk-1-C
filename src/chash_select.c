//支払方法は現金、クレジットカード、電子マネーの三種類。文字列Buy_Chackと順に
//1~7の数字をmain.cに返す。
#include<stdio.h>
int sceneNumber=0;
int cardNumber=2;
int payNumber=5;
char SceneName[64]={};

void *start_Scene(){
printf("========================= メニュー =========================\n");
printf("お支払い方法を選択してください\n");
printf("1:現金\n");
printf("2:クレジットカード\n");
printf("3:QRコード\n\n\n");
printf("=============================================================\n");
printf("支払方法の番号を入力してください\t:\t");
scanf("%d",&sceneNumber);

if(sceneNumber==1){
char SceneName[]="Buy_Chack";
return sceneNumber;
}else if(sceneNumber ==2){
printf("========================= メニュー =========================\n");
printf("使用するカードを選択してください\n");
printf("1:三井住友カード\n");
printf("2:JCB\n");
printf("3:三菱UFJカード\n\n\n");
printf("=============================================================\n");
printf("使用するカードの番号を入力してください\t:\t");
scanf("%d",&cardNumber);

if(cardNumber == 1){
char SceneName[]="Buy_Chack";
return cardNumber;
}else if(cardNumber ==2){
char SceneName[]="Buy_Chack";
return cardNumber+1;
}else if(cardNumber == 3){
char SceneName[]="Buy_Chack";
return cardNumber+2;
}
}else if(sceneNumber ==3){
printf("========================= メニュー =========================\n");
printf("使用するアプリを選択してください\n");
printf("1:PayPay\n");
printf("2:楽天ペイ\n");
printf("3:d払い\n\n\n");
printf("=============================================================\n");
printf("使用するアプリの番号を入力してください\t:\t");
scanf("%d",&payNumber);
if(payNumber == 1){
char SceneName[]="Buy_Chack";
return payNumber;
}else if(payNumber ==2){
    char SceneName[]="Buy_Chack";
return cardNumber+1;
}else if(payNumber == 3){
char SceneName[]="Buy_Chack";
return cardNumber+2;
}
}