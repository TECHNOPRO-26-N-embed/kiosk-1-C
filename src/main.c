#include <stdio.h>
#include"start.c"
int sceneNumber=0;

int sceneChanger(int scene)
{

    sceneNumber=scene;

    return scene;

}

int main(void) {
printf("========================= メニュー =========================\n");
printf("\n\n\n\n\n");



printf("1: 読み取り画面へ\t\t\t");
printf("0: 終了");



printf("\n");
printf("=============================================================\n");

printf("番号を入力\t:\t");
scanf("%d",&sceneNumber);



switch(sceneNumber)
{
    case 1:
    action();
    break;
    case 9:
    printf("終了します\n");
    break;
    default:
    printf("無効な選択です\n");     


}

return 0;

}