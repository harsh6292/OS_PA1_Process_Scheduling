/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	int itemA, itemB, itemC,proc;
	int num, i;
	//STATWORD ps;
	optr = &proctab[currpid];
	
	if(sched_type==RANDOMSCHED)
	{
		/*if(count_random==0)
		{
			count_random++;
			enqueue(dequeue(0),queueC_tail);
		}*/
		
		if((!nonempty(queueC_head)) && (!nonempty(queueC_head)) && (!nonempty(queueC_head)))
		{
			return OK;
		}
		
		proctab[currpid].quantumLeft = preempt;	//quantum used in last execution
		if(switched_to_randomsched==1)
		{
			switched_to_randomsched=0;
			proctab[currpid].procCpuTicks += (proctab[currpid].assignedQuantum - preempt);
		}
		else
		{
			proctab[currpid].procCpuTicks += (QUANTUM - preempt);
		}
		//proctab[currpid].procCpuTicks += (QUANTUM - preempt);
		
		/*Make current process as ready*/
		if (optr->pstate == PRCURR) 
		{
			//kprintf("\nen inside ");

			optr->pstate = PRREADY;
			if(((optr->pprio)>=66) && ((optr->pprio)<=99))
			{
				//kprintf("\nReady Queue A, %s", optr->pname);
				enqueue(currpid, queueA_tail);
			}
		
			else if(optr->pprio>=33 && optr->pprio<=65)
			{
				//kprintf("\nReady Queue b, %s", optr->pname);
				enqueue(currpid, queueB_tail);
			}
		
			else if(optr->pprio>=0 && optr->pprio<=32)
			{
				//kprintf("\nReady Queue c, %s", optr->pname);
				enqueue(currpid, queueC_tail);
				//enqueue(0,queueC_tail);
			}
		}
		
		//srand(1);
		generate_random:
			num = rand();
			//kprintf("\n rand 1: %d", num);
			num = (num%97);
			//kprintf("\n normalized rand 1: %d", num);
		
		
		if(num>=50)
		{
		
			if(nonempty(queueA_head))
			{	
				itemA = getfirst(queueA_head);
				//kprintf("\nresched Queue A, %d",itemA);

				nptr = &proctab[ (currpid = itemA)];//getfirst(queueA_head)) ];
				nptr->pstate = PRCURR;		/* mark it currently running	*/
				#ifdef	RTCLOCK
					preempt = QUANTUM;		/* reset preemption counter	*/
				#endif
		
				ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		
				/* The OLD process returns here when resumed. */
				return OK;

			}
			else
				goto generate_random;//resched();
		}
		
		else if(num>20 && num <50)
		{
		
			if(nonempty(queueB_head))
			{
				itemB = getfirst(queueB_head);
				//kprintf("\nresched Queue B, %d, %s",itemB, proctab[itemB].pname);

				nptr = &proctab[ (currpid = itemB)];//getfirst(queueB_head)) ];
				nptr->pstate = PRCURR;		/* mark it currently running	*/
				#ifdef	RTCLOCK
					preempt = QUANTUM;		/* reset preemption counter	*/
				#endif
		
				ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		
				/* The OLD process returns here when resumed. */
				return OK;

			}
			else
				goto generate_random;//resched();
		}
		
		else if(num<=20)
		{
			
			if(nonempty(queueC_head))
			{
				itemC = getfirst(queueC_head);
				//kprintf("\nresched Queue C, %d, %s",itemC, proctab[itemC].pname);

				nptr = &proctab[ (currpid = itemC)];//getfirst(queueC_head)) ];
				nptr->pstate = PRCURR;		/* mark it currently running	*/
				#ifdef	RTCLOCK
					preempt = QUANTUM;		/* reset preemption counter	*/
				#endif
		
				ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		
				/* The OLD process returns here when resumed. */
				return OK;

			}
			else
				goto generate_random;//resched();
		}
	}



	
	
	else if(sched_type == LINUXSCHED)
	{
		//int usedPreempt = preempt;
		int highProc;
		int oldPriority = proctab[currpid].pprio;	//save the current process' priority so that the effect of chprio() or change in priority to the quantum
													// or goodness is reflected in new epoch
		//int procGoodness = 0;
		int startNewEpoch = 1;
		//disable(ps);
		proctab[currpid].quantumLeft = preempt;
		if(switched_to_linuxsched==1)
		{
			proctab[currpid].runnableProc = 1;
			switched_to_linuxsched=0;
			proctab[currpid].procCpuTicks += (QUANTUM - preempt);
			proctab[currpid].assignedQuantum = proctab[currpid].assignedQuantum -(QUANTUM - preempt);
		}
		else
		{
			proctab[currpid].procCpuTicks += (proctab[currpid].assignedQuantum - preempt);
			proctab[currpid].assignedQuantum = proctab[currpid].quantumLeft;
		}
		
		
		if(optr->pstate == PRCURR)
		{
			optr->pstate = PRREADY;
			insert(currpid, rdyhead, proctab[currpid].procGoodness);
		}
		
		
			highProc = rdytail;

			while(q[highProc].qprev!=rdyhead  && q[highProc].qprev>=0 && q[highProc].qprev<NPROC)				//q[temp].qnext!=rdytail && q[temp].qnext>=0 && q[temp].qnext<NPROC
			{
				//kprintf("\t pbdsf%s",proctab[q[highProc].qprev].pname);
				
				
				if((proctab[q[highProc].qprev].runnableProc == 1) && (proctab[q[highProc].qprev].assignedQuantum > 0 ) )	//q[highProc].qnext!=0 && 
				{
					//kprintf("\n bacha hai : %d, %s",q[highProc].qprev, proctab[q[highProc].qprev].pname);
					startNewEpoch = 0;
					break;
				}
						
				highProc = q[highProc].qprev;
			}

		
		
		if(startNewEpoch == 1)
		{
			//kprintf("\nNew Epoch started---*******");
			for(i=0; i<NPROC; i++)
			{
				if(proctab[i].pstate != PRFREE)
				{
					oldPriority = proctab[i].pprio;	//prevent priority change effect in calculation of quantum
					//proctab[i].runnableProc = 1;
					
					//calculate quantum of each process either sleeping/waiting or ready
					if(proctab[i].runnableProc == 1)
					{
						if(proctab[i].assignedQuantum > 0)
						{
							proctab[i].assignedQuantum = (oldPriority + (proctab[i].assignedQuantum /2));
							
							proctab[i].procGoodness = (oldPriority + proctab[i].quantumLeft);
							//kprintf("\nNew quantum and goodness %d, %d, %s",proctab[i].assignedQuantum, proctab[i].procGoodness, proctab[i].pname);
						}
						else
						{
							proctab[i].assignedQuantum = oldPriority;
							
							proctab[i].procGoodness = 0;//(oldPriority);
							//kprintf("\nused up quantum %d, %d, %s",proctab[i].assignedQuantum, proctab[i].procGoodness, proctab[i].pname);
						}
					}
					proctab[i].runnableProc = 1;
					//kprintf("\nFinal values of quantum and goodness %d, %d, %s",proctab[i].assignedQuantum, proctab[i].procGoodness, proctab[i].pname);
					
					dequeue(i);
					insert(i, rdyhead, proctab[i].procGoodness);
					
					
				}
			}
			
			
		}
		
		//goodness is to be calculated for process in same epoch also ? -> no
	
		highProc = rdytail;
		
		
		while(q[highProc].qprev!=rdyhead && q[highProc].qprev>=0 && q[highProc].qprev<NPROC)
		{
			
			/*kprintf("\n Highest proc based on goodness %d, %d, %d", q[highProc].qprev, proctab[q[highProc].qprev].procGoodness, proctab[q[highProc].qprev].assignedQuantum);
			if(proctab[q[highProc].qprev].pstate == PRREADY)
			{
				kprintf("\nstate: PRREADY, %d", proctab[q[highProc].qprev].runnableProc);
				
			}*/
			if( (proctab[q[highProc].qprev].runnableProc == 1) && (proctab[q[highProc].qprev].assignedQuantum > 0 ) )//&& (proctab[q[highProc].qprev].pstate==PRREADY) && q[highProc].qprev!=0 )
			{
				//kprintf(" \ninside----------- %d", currpid);
				currpid = dequeue(q[highProc].qprev);
				//kprintf(" \nnew pid----------- %d", currpid);
				//if(currpid != 0)
				//{
					nptr = &proctab[currpid];
					nptr->pstate = PRCURR;		/* mark it currently running	*/
				#ifdef	RTCLOCK
					preempt = nptr->assignedQuantum;		/* reset preemption counter	*/
				#endif
	
					ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		
					/* The OLD process returns here when resumed. */
					return ;// OK;
					break;
				}
			
			else if( (q[rdyhead].qnext == 0) && (q[rdytail].qprev== 0))
				{
					//proctab[currpid].assignedQuantum = QUANTUM;
			//	kprintf(" \ninside----------- %d", currpid);
					nptr = &proctab[(currpid=0)];
					nptr->pstate = PRCURR;		/* mark it currently running	*/
				#ifdef	RTCLOCK
					preempt = QUANTUM;		/* reset preemption counter	*/
				#endif
					ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		
					/* The OLD process returns here when resumed. */
					return;// OK;
					break;
				}
				highProc = q[highProc].qprev;
			}
	
	}

	
	
	

	else if(sched_type!=RANDOMSCHED && sched_type!=LINUXSCHED)
	{

		/* no switch needed if current process priority higher than next*/
	
	
		if ( ( optr->pstate == PRCURR) && (lastkey(rdytail)<optr->pprio)) 
		{
			return(OK);
		}
	
		/* force context switch */

		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		proctab[currpid].quantumLeft = preempt;
		proctab[currpid].procCpuTicks += (QUANTUM - preempt);
		/* remove highest priority process at end of ready list */

		nptr = &proctab[ (currpid = getlast(rdytail)) ];
		nptr->pstate = PRCURR;		/* mark it currently running	*/
	#ifdef	RTCLOCK
		preempt = QUANTUM;		/* reset preemption counter	*/
	#endif
	
		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
		/* The OLD process returns here when resumed. */
		return OK;
	}
}
