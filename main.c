#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/* Load Networking Headers */
#ifdef __WIN32
  #include <winsock.h>          // for windows
  #define socklen_t int
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/ioctl.h>
  #include <sys/un.h>
  #include <net/if.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>        // for UNIX
  #include <netdb.h>
#endif

#include "vars.h"

int main(void) {
    int i, n=0, s, ss, cont=0,pars_fl=0;
    struct sockaddr_in addr;
    struct hostent *he;
    struct in_addr **addr_list;
    char SendBuf[MAX]={0}, RecvBuf[MAX]={0},Buff[8][40]={0},TempBuff[40]={0};
    char * pvm; // Private Message

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

    // parse socket
    while(i) {
      n=0; // Clear the buffers
      while(n<MAX) {
        RecvBuf[n]='\0';
        SendBuf[n]='\0';
        n++;
      }
      if(cont==0) {
        sleep(2);
      }
      i=recv(s, RecvBuf, MAX, 0);
      if(DEBUG>0) printf("%d bytes received: \n%s\nConnectivity : %d\n", i, RecvBuf,cont);
      // Check for Ping-Pong

      pvm=strtok(RecvBuf," :!~\n");
      n=0;
      if(DEBUG>0) printf ("\n\n====-DEBUG-====\n");
      while(pvm!=NULL) {
        if(DEBUG>0) printf ("%d: %s\n",n,pvm);
        sprintf(Buff[n],"%s",pvm);
        pvm=strtok (NULL, " :!~\n");
        n++;
      }
      if(DEBUG>0) printf("====- END -====\n\n");

      if(strcmp("PING", Buff[0])==0 || strcmp("PING", Buff[6])==0) {
        sprintf(SendBuf,"PONG %s%s\r\n", Buff[1],Buff[7]);
        send(s,SendBuf,strlen(SendBuf),0);
        if(write(s, SendBuf, MAX, 0)<0) {
          perror("PING-PONG!");
        }
      }

      if(strncmp("%insult", Buff[4],7)==0) {
        if(n>4) {
          sprintf(SendBuf,"PRIVMSG %s : Hey %s - %s says %s! (%n)\r\n",CHAN, Buff[0], Buff[5],n);
        }
        else {
          sprintf(SendBuf,"PRIVMSG %s : %s, wtf is wrong with you, you are so fudge packing ignorant you ca not even provide me with a name to insult?  Jackass...\r\n",CHAN, Buff[0]);
        }
        if(send(s,SendBuf,strlen(SendBuf),0)<0) {
          perror("Insult");
        }
        sleep(3); // Try to avoid flooding...
      }
      if(strncmp("%quit", Buff[4],5)==0) {
        // ABORT! ABORT!
        sprintf(SendBuf,"QUIT Requested... \r\n : Requested...\r\n");
        if(send(s,SendBuf,strlen(SendBuf),0)<0) {
          perror("Quit Request");
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
          if(DEBUG>0) printf("PRIVMSG from %s\n",Buff[0]);
          if(strcmp("VERS", TempBuff)==0) {
            sprintf(SendBuf, "NOTICE %s : \001VERSION %s : v%s : GHCi\001",Buff[0], VER_STR, VER);
            if(DEBUG>0) printf("Returning Version Request...\n");
            send(s,SendBuf,strlen(SendBuf),0);
            if(write(s, SendBuf, MAX, 0)<0) {
              perror("version");
            }
          }

          if(strcmp("PING",TempBuff)==0) {
            sprintf(SendBuf,"NOTICE %s : PONG %s\r\n",Buff[0],VER);
            if(DEBUG>0) printf("Returning Ping Request...\n");
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
