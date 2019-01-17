#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 10 //登録上限数
#define N 32 

//構造体の宣言
typedef struct data_t{
  char name[N]; //名前
  char mail[N]; //メールアドレス
  char number[N]; //電話番号
}Data;

//関数のプロトタイプ宣言
int menu(void);
void trimEnd(char str[]);
void inputData(Data phone[], int *account);
void printData(Data phone[], int index);
void printAll(Data phone[], int account);
void searchName(Data phone[], int account);
void searchMail(Data phone[], int account);
void setName(char str[]);
void sortName(Data phone[], int account);
void saveData(Data phone[], int account);
void loadData(Data phone[], int *account);

int main(void){
  Data phone[MAX];
  int account = 0; //構造体の要素の番地
  int select_menu;

  do{
    select_menu = menu();
    switch(select_menu){
      case 1: //新規登録
        printf("[新規登録]\n");
        inputData(phone, &account);
        //printf("テスト: %s account = %d\n", phone[0].name, account);
        break;
      case 2: //一覧表示
        printf("[一覧表示]\n");
        printAll(phone, account);
        break;
      case 3: //名前検索
        printf("[名前検索]\n");
        searchName(phone, account);
        break;
      case 4: //メアド検索
        printf("[メールアドレス検索]\n");
        searchMail(phone, account);
        break;
      case 5: //セーブ
        saveData(phone, account);
        break;
      case 6: //ロード
        loadData(phone, &account);
        break;
      default: //終了
        printf("※終了します\n");
        break;
    }
  }while(select_menu);

  return 0;
}

//メニュー
int menu(void){
  int select_menu;
  printf("\n**メニュー***************\n");
  printf("1.新規登録 2.一覧表示\n3.名前検索 4.メールアドレス検索\n5.セーブ   6.ロード\n0.終了\n");
  printf("*************************\n");
  do{
    printf("番号を入力してください: ");
    scanf("%d", &select_menu);
    (void)getchar(); //バッファに残った改行コードの読み捨て
  }while(select_menu < 0 || 6 < select_menu);
  printf("\n");

  return select_menu;
}

//文字列の改行コードを取り除く
void trimEnd(char str[]){
  int i = 0;
  while(str[i] != '\n' && i < strlen(str)){
    i++;
  }
  str[i] = '\0';
}

//データ入力
void inputData(Data phone[], int *account){
  if(MAX < *account - 1){
    printf("※登録数が上限に達しています\n");
    return;
  }else{
    printf("名前を入力してください: ");
    fgets(phone[*account].name, N, stdin);
    trimEnd(phone[*account].name);

    setName(phone[*account].name);

    printf("メールアドレスを入力してください: ");
    fgets(phone[*account].mail, N, stdin);
    trimEnd(phone[*account].mail);

    printf("電話番号を入力してください: ");
    fgets(phone[*account].number, N, stdin);
    trimEnd(phone[*account].number);
    
    (*account)++;

    sortName(phone, *account);
  }
  printf("※登録が完了しました\n");
}

//ひとつのアカウントのデータを表示
void printData(Data phone[], int index){
  printf("----------------------\n");
  printf("*名前: %s\n", phone[index].name);
  printf("*メールアドレス: %s\n", phone[index].mail);
  printf("*電話番号: %s\n", phone[index].number);
}

//データの一覧表示
void printAll(Data phone[], int account){
  int i;
  for(i = 0; i < account; i++){
    printData(phone, i);
  }
  printf("----------------------\n");
}

//名前検索
void searchName(Data phone[], int account){
  char search_str[N];
  int i;
  int flag = 0; //検索した名前があるかどうか

  printf("検索する名前を入力してください: ");
  fgets(search_str, N, stdin);
  trimEnd(search_str);
  setName(search_str);

  for(i = 0; i < account; i++){
    if(strstr(phone[i].name, search_str) != NULL){
      printData(phone, i);
      flag = 1;
    }
  }

  if(flag){
    printf("----------------------\n");
  }else{
    printf("\n※入力された名前が見つかりませんでした\n");
  }
}

//メアド検索
void searchMail(Data phone[], int account){
  char search_str[N];
  int i;
  int flag = 0; //検索したメアドがあるかどうか

  printf("検索する文字列を入力してください: ");
  fgets(search_str, N, stdin);
  trimEnd(search_str);

  for(i = 0; i < account; i++){
    if(strstr(phone[i].mail, search_str) != NULL){
      printData(phone, i);
      flag = 1;
    }
  }
  if(flag){
    printf("----------------------\n");
  }else{
    printf("\n※入力された文字列を含むメールアドレスが見つかりませんでした\n");
  }
}

//入力された名前の先頭を大文字、他を小文字に
void setName(char str[]){
  int i;
  for(i = 0; i < strlen(str); i++){
    if(i == 0 || str[i - 1] == ' '){ 
      str[i] = toupper(str[i]); //大文字変換
    }else{
      str[i] = tolower(str[i]); //小文字変換
    }
  }
}

//名前のアルファベット順に並び替える
void sortName(Data phone[], int account){
  Data tmp; //入れ替え用構造体
  int i;
  int flag = 0;

  do{
    flag = 0;
    for(i = 0; i < account - 1; i++){
      if(strcmp(phone[i].name, phone[i + 1].name) > 0){
        //printf("unch\n");
        tmp = phone[i];
        phone[i] = phone[i + 1];
        phone[i + 1] = tmp;
        flag = 1;
      }
    }
  }while(flag);
}

//データの読み込み
void loadData(Data phone[], int *account){
  FILE *fp;
  *account = 0;

  fp = fopen("data.txt", "r"); //読み込み専用でファイルを開く

  if(fp == NULL){
    printf("※ファイルが開けませんでした\n");
    return;
  }

  while(fgets(phone[*account].name, N, fp) != NULL){
    fgets(phone[*account].mail, N, fp);
    fgets(phone[*account].number, N, fp);

    trimEnd(phone[*account].name);
    trimEnd(phone[*account].mail);
    trimEnd(phone[*account].number);

    (*account)++;
  }
  fclose(fp);
  printf("※ロードしました\n");
}

//データの書き込み
void saveData(Data phone[], int account){
  FILE *fp;
  int i = 0;

  fp = fopen("data.txt", "w"); //書き込み専用でファイルを開く

  if(fp == NULL){
    printf("※ファイルが開けませんでした\n");
    return;
  }

  for(i = 0; i < account; i++){
    fprintf(fp, "%s\n%s\n%s\n", phone[i].name, phone[i].mail, phone[i].number);
  }
  fclose(fp);
  printf("※セーブしました\n");
}
