/* ready.c - ready */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>

/*------------------------------------------------------------------------
 * ready  --  make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
int ready(int pid, int resch)
{
	register struct	pentry	*pptr;
	//int itm,it2;

	if (isbadpid(pid))
		return(SYSERR);
	pptr = &proctab[pid];
	pptr->pstate = PRREADY;

	if(sched_type==RANDOMSCHED)
	{
		
		if(((pptr->pprio)>=66) && ((pptr->pprio)<=99))
		{
			enqueue(pid, queueA_tail);
		}
		
		else if(((pptr->pprio)>=33) && ((pptr->pprio)<=65))
		{
			enqueue(pid, queueB_tail);
		}
		
		else if(((pptr->pprio)>=0) && ((pptr->pprio)<=32))
		{
			enqueue(pid, queueC_tail);
		}
		/*itm = getfirst(queueA_head);
		enqueue(itm, queueA_tail);
		it2 = getlast(queueA_tail);

		kprintf("\n first and last :%d, %d", itm, it2);*/

		if (resch)
			resched();
		return(OK);
		
	}
	
	
	else if(sched_type==LINUXSCHED)
	{
		insert(pid,rdyhead,pptr->procGoodness);
		if (resch)
			resched();
		return(OK);
	}
	
	else if(sched_type!=RANDOMSCHED && sched_type!=LINUXSCHED)
	{
		insert(pid,rdyhead,pptr->pprio);
		if (resch)
			resched();
		return(OK);
	}
}
