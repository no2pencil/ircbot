#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/* Load Networking Headers */
#if defined(linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h> // for UNIX
#include <netdb.h>
#else
#include <winsock.h> // for windows
#define socklen_t int
#endif

#include "vars.h"
#include "functions.h"

void init_insults(int *i);

int main(int argc, char *argv[]) {
    int i, a=0, n=0, s, ss, cont=0,pars_fl=0, ins=0, inc=0, png=0, opt=0;
    bool DEBUG=false, INSULT=false;
    struct sockaddr_in addr;
    struct hostent *he;
    struct in_addr **addr_list;
    char SendBuf[MAX]={0}, RecvBuf[MAX]={0},Buff[8][40]={0},TempBuff[40]={0};
    char ch='\0', *insults[MAX], * pvm; // Private Message
    char *actions[MAX], * actn; 

    /* Before anything else, lets start that timer! */
    srand(time(NULL));

    /* Check for DEBUG argument */
    if(argc > 1) {
      /* Parse Arguments like a boss */
      while((opt=getopt(argc, argv, "d:i:"))!=-1) {
/*
        switch(opt) {
          case 'd':
            printf("enabling Debug...\n");
            DEBUG=true;
          break;
          case 'i':
            INSULT=true;
          break;
        }
*/
        if(opt=='d') {
          DEBUG=true;
        }
        if(opt=='i') {
          INSULT=true;
        }
      }
      n=0;
    }

    if ((s=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* DNS HOST ADDR */
    if((he=gethostbyname(SERVERHOST))==NULL) {
      perror("NS Translate");
      exit(1);
    }

    printf("Connecting to Host : %s \nIP Address : ",he->h_name);

    addr_list=(struct in_addr **)he->h_addr_list;
    for(i=0; addr_list[i]!=NULL; i++) {
        printf("%s ", inet_ntoa(*addr_list[i]));
    }
    printf("\n\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(inet_ntoa(*addr_list[0]));
    addr.sin_port=htons(PORT);

    ss=connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if(ss<0) {
      perror("connect");
      exit(1);
    }

    printf("Connected to %s.\n",SERVERHOST);

    /* Load Action File */
    if(INSULT) {
      init_insults(&n);
      //if(loadactions(ACTION_FILE)!=0) perror("Actions file");
      for(n=0;n<43;n++) {
        actions[n]=action[n];
        printf("\n %d : %s\n",n,action);
      }

      /* Load Inuslts File */
      if(loadinsults(INSULT_FILE)!=0) perror("Insults file");
      for(n=0;n<43;n++){
        insults[n]=insult[n];
        printf("\n %d : %s\n",n,insult);
        //printf("\n %d : %s\n",n,insults);
      }
    }

    // parse socket
    while(i) {
      n=0; // Clear the buffers
      while(n<MAX) {
        RecvBuf[n]='\0';
        SendBuf[n]='\0';
        n++;
      }
      if(cont==0) {
        printf("zzZZzzZZ...\n");
        sleep(2);
      }
      i=recv(s, RecvBuf, MAX, 0);
      if(DEBUG) printf("%d bytes received: \n%s\nConnectivity : %d\n", i, RecvBuf,cont);
      // Check for Ping-Pong

      pvm=strtok(RecvBuf," :!~\n");
      n=0;
      if(DEBUG) printf ("\n\n====-DEBUG-====\n");
      while(pvm!=NULL) {
        if(DEBUG) printf ("%d: %s\n",n,pvm);
        sprintf(Buff[n],"%s",pvm);
        pvm=strtok (NULL, " :!~\n");
        n++;
      }
      if(DEBUG) printf("====- END -====\n\n");

      printf("\nzzZZzzZZ....%d\n",png);
      sleep(2);

      if(strcmp("PING", Buff[png])==0) {
        printf("\nPING Detected... Sending PONG! %s\n",Buff[1]);
        sprintf(SendBuf,"PONG %s%s\r\n", Buff[1],Buff[7]);
        send(s,SendBuf,strlen(SendBuf),0);
        if(write(s, SendBuf, MAX, 0)<0) {
          perror("PING-PONG!");
        }
      }

/*
====-DEBUG-====
0: no2p
1: no2pencil@mail.akroncdnr.com
2: PRIVMSG
3: #dreamincode
4: oh
5: fuck
====- END -====
*/

      n=4;
      while(n<28) {
        if(strncmp("kool-aid",Buff[n],8)==0) {
          for(ins=1;ins<13;ins++)inc=getrand(1,99);
          if(inc>75) {
            sprintf(SendBuf,"PRIVMSG %s : %s had better back the fuck off my kool-aid! \r\n",CHAN, Buff[0]);
            if(send(s,SendBuf,strlen(SendBuf),0)<0) {
              perror("There was an error with your Kool-Aid");
            }
          }
        }
        if((strncmp("alive?",Buff[n],5)==0) || (strncmp("awake?",Buff[n],5)==0)) {
          sleep(2); // Try to avoid flooding...
          // Take a random chance at bothering to reply...
          for(ins=1;ins<13;ins++)inc=getrand(1,99);
          if(inc>35) {
            sprintf(SendBuf,"PRIVMSG %s : Shhh %s! I'm require silence for fapping! \r\n",CHAN, Buff[0]);
            if(send(s,SendBuf,strlen(SendBuf),0)<0) {
              perror("Help");
            }
          }
          else {
            sprintf(SendBuf,"PRIVMSG %s : Not now %s! I'm... bating! \r\n",CHAN, Buff[0]);
            if(send(s,SendBuf,strlen(SendBuf),0)<0) {
              perror("Help");
            }
          }
        }
        if(strncmp("help", Buff[n],4)==0) {
          sleep(2); // Try to avoid flooding...
          // Take a random chance at bothering to reply...
          for(ins=1;ins<13;ins++)inc=getrand(1,99);
          if(inc>35) {
            sprintf(SendBuf,"PRIVMSG %s : There is no help for you %s. \r\n",CHAN, Buff[0]);
            if(send(s,SendBuf,strlen(SendBuf),0)<0) {
              perror("Help");
            }
          }
        }
        n++;
      }

      if(strncmp("LOL", Buff[4],3)==0) {
        sleep(2); // Try to avoid flooding...
        sprintf(SendBuf,"PRIVMSG %s : (L)ick (O)ld (L)adies!!\r\n",CHAN);
        if(send(s,SendBuf,strlen(SendBuf),0)<0) {
          perror("Lick Old Ladies");
        }
      }

      if(INSULT) {
        for(i=0;i<38;i++) {
          for(n=0;n<12;n++) { 
            if(actions[i][0]!='\0') {
              if(strcmp(actions[i],Buff[n])==0) {
                printf("Found %s in %s\n",actions[i],Buff[n]);
                a=getrand(0,5);
                sprintf(SendBuf,"PRIVMSG %s :\001ACTION %ss %s right in the %s!\r\n",CHAN, action[i], Buff[0],place[a]);
                if(send(s,SendBuf,strlen(SendBuf),0)<0) {
                  perror("Action");
                }
                n=1000;
                i=1000;
              }
            }
          }  
        }
      }

      if(strncmp("insult", Buff[4],6)==0) {
        sleep(2); // Try to avoid flooding...
        if(INSULT) {
	  i=getrand(1,44);
          sprintf(Buff[6],"%s",insult[i]);
          if(Buff[6][0]=='\0') {
            sprintf(SendBuf,"PRIVMSG %s : Hey %s, why don't you do it yourself?!\r\n",CHAN, Buff[0]);
	    printf("Insult failed, loaded \n%d:%s\n",i,Buff[6]);
          } else {
            sprintf(SendBuf,"PRIVMSG %s : Hey %s - %s (courtesy %s)! \r\n",CHAN, Buff[5], Buff[6], Buff[0]);
          }
        } else {
          sprintf(SendBuf,"PRIVMSG %s : Insults are not current enabled! \r\n",Buff[0]);
        }
        if(send(s,SendBuf,strlen(SendBuf),0)<0) {
          perror("Insult");
        }
      }

      if(strncmp("loadActions", Buff[4],11)==0) {
        if(loadactions(ACTION_FILE)!=0) perror("Actions file");
        else {
          sprintf(SendBuf,"PRIVMSG %s : Action File Loaded...\r\n",Buff[0]);
          for(n=0;n<43;n++) {
            actions[n]=action[n];
          }
          if(send(s,SendBuf,strlen(SendBuf),0)<0) {
            perror("Load Actions");
          }
        }
      }

      if(strncmp("loadInsults", Buff[4],11)==0) {
        /* Dynamically Load Inuslts File */
        if(loadinsults(INSULT_FILE)!=0) perror("Insults file");
        else {
          sprintf(SendBuf,"PRIVMSG %s : File loaded... \r\n ",Buff[0]);
          for(n=0;n<MAX;n++){
            insults[n]=insult[n];
          }
          if(send(s,SendBuf,strlen(SendBuf),0)<0) {
            perror("Load Insults");
          }
        }
      }

      if(strncmp("%quit", Buff[4],5)==0) {
        // ABORT! ABORT!
        if(strncmp("#dreamincode",Buff[3],12)!=0) { /* only quit if message is private */
          sprintf(SendBuf,"QUIT Requested... \r\n : Requested...\r\n");
          if(send(s,SendBuf,strlen(SendBuf),0)<0) {
            perror("Quit Request");
          }
        }
      }

      if(cont==0) {
        sprintf(SendBuf, "NICK %s\r\nUSER %s %s %s : TestBot \r\nJOIN %s\r\n", NICK, USER, IDENT, IDENT, CHAN);
        if(write(s, SendBuf, MAX, 0)<0) {
          perror("Trasmit");
        }
        else cont++;
      }
      if(cont==1) {
        sprintf(SendBuf, "JOIN %s\r\n", CHAN);
        if(write(s, SendBuf, MAX, 0)<0) {
          perror("Join");
        }
        sleep(3); // More flood protection
/*
        sprintf(SendBuf, "PRIVMSG no2pencil : %s\r\n", MESSAGE);
        if(write(s, SendBuf, MAX, 0)<0) {
          perror("Private Message");
        }
*/

        if(strcmp("PRIVMSG", Buff[2])==0) {
          sprintf(TempBuff,"%c%c%c%c",Buff[4][1],Buff[4][2],Buff[4][3],Buff[4][4]);
          if(DEBUG) printf("PRIVMSG from %s\n",Buff[0]);
          if(strcmp("VERS", TempBuff)==0) {
            sprintf(SendBuf, "NOTICE %s :\001VERSION %s : v%s : GHCi",Buff[0], VER_STR, VER);
            if(DEBUG) printf("Returning Version Request...\n");
            send(s,SendBuf,strlen(SendBuf),0);
            if(write(s, SendBuf, MAX, 0)<0) {
              perror("version");
            }
          }

          if(strcmp("PING",TempBuff)==0) {
            sprintf(SendBuf,"NOTICE %s : PONG \r\n",Buff[0]);
            if(DEBUG) printf("Returning Ping Request...\n");
            send(s,SendBuf,strlen(SendBuf),0);
            if(write(s, SendBuf, MAX, 0)<0) {
              perror("PING-PONG!");
            }
          }
        }
      }
    }

    if(cont>0) printf("Client exited\n");
    else printf("Unable to connect\n");

    if(s) close(s);
    return 0;
}

void init_insults(int *i) {
    printf("Passed : %c\n", *i); 
    /* Lame, I know */
    /*
    sprintf(place[0],"face");
    sprintf(place[1],"teeth");
    sprintf(place[2],"ass");
    sprintf(place[3],"head");
    sprintf(place[4],"back");
    sprintf(place[5],"neck");
    */
}
