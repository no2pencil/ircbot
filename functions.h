/*
 * Functions Header
 */

#include <time.h>  // Required for srand

/* Global var */
char insult[MAX][MAX]={0};

int getrand(int min,int max){
     return(rand()%(max-min)+min);
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
