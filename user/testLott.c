#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#define check(exp, msg) if(exp) {} else {\
   printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
   exit();}
#define PROC 7

void spin()
{
	int i = 0;
  int j = 0;
  int k = 0;
	for(i = 0; i < 50; ++i)
	{
		for(j = 0; j < 10000000; ++j)
		{
      k = j % 10;
      k = k + 1;
    }
	}
}
int main(int argc, char *argv[]){
  struct pstat st;
  int i;
  settickets(100);
  spin();
  getpinfo(&st);
  for(i=0; i<NPROC; i++)
    printf(1, "pid: %d tickets: %d ticks: %d inuse: %d\n", st.pid[i], st.tickets[i], st.ticks[i], st.inuse[i]);
  exit();
}
