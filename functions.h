/*
 * Functions Header
 */

#include <time.h>  // Required for srand

int getrand(int min,int max){
     return(rand()%(max-min)+min);
}
