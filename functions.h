/*
 * Functions Header
 */

#include <time.h>  // Required for srand

/* Global var */
char insult[MAX][MAX]={0};
char action[MAX][MAX]={0};
char place[6][MAX]={0};

int getrand(int min,int max){
     return(rand()%(max-min)+min);
}

int loadactions(char filename[MAX]) {
  int act_inc=0,act_str, act_total;
  char ch='\0', TempBuff[40]={0};
  FILE *fp;
  /* Setup up Actions */
  fp=fopen(filename, "r");
  if(!fp) return 1;

  while(ch!=EOF) {
    ch=fgetc(fp);
    if(ch!='\n') {
      TempBuff[act_inc]=ch;
      printf("%c",TempBuff[act_inc]);
      act_inc++;
    }
    else {
      act_inc=0;
      sprintf(action[act_str], "%s",TempBuff);
      act_str++;
      while(act_inc<40) {
        TempBuff[act_inc]='\0';
        act_inc++;
      }
      act_inc=0;
      act_total++;
    }
  }
  printf("Loaded %d actions from the action text file.\n",act_total);
  fclose(fp);
  return 0;
}

int loadinsults(char filename[MAX]) {
  int inc=0,ins=0;
  char ch='\0', TempBuff[40]={0};
  FILE *fp;
  printf("Connected to %s.\n",SERVERHOST);

  /* Set up all the insults */
  fp=fopen(filename, "r");
  if(!fp) {
    return 1;
  }

  while(ch!=EOF) {
    ch=fgetc(fp);
    if(ch!='\n') {
      TempBuff[inc]=ch;
      printf("%c",TempBuff[inc]);
      inc++;
    }
    else {
      inc=0;  // reset the character coutner
      sprintf(insult[ins], "%s",TempBuff);
      ins++;  // incriment the string counter
      while(inc<40) {
        TempBuff[inc]='\0'; // Null the element
        inc++;
      }
      inc=0; // reset the character counter, again
    }
  }

  printf("Loaded insult text.\n");

  fclose(fp);

  return 0;
}
