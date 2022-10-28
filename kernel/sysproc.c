#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
#include "pstat.h"
#include "spinlock.h"


/* The following code has been added by Subhrangsu Bose(sxb210184)
** Defining settickets(int) and getpinfo(struct *pstat)
*/
struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

  /* End of code added/modified */
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

/* The following code has been added by Subhrangsu Bose(sxb210184)
** Defining settickets(int) and getpinfo(struct *pstat)
*/
  
int
sys_settickets(void)
{
  int ticketno;
  argint(0, &ticketno);
  if(ticketno <= 0)
    return -1;
  else{
    proc->tickets = ticketno;
    }
    return 0;
}


int
sys_getpinfo(void)
{
  struct pstat* ps;
  struct proc* p;
  
  argptr(0, (void*)&ps, sizeof(*ps));
  
  if(ps == NULL)
    return -1;
  
  acquire(&ptable.lock);
  for(p=ptable.proc;p!=&(ptable.proc[NPROC]);p++){
    const int i = p-ptable.proc;
    if(p->state != UNUSED){
      ps->pid[i] = p->pid;
      ps->ticks[i] = p->ticks;
  	  ps->tickets[i] = p->tickets;
	    ps->inuse[i] = p->inuse;
     }
    }
    release(&ptable.lock);
    return 0;
  }

  /* End of code added/modified */ 


int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
