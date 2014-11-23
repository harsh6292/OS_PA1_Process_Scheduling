#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<stdio.h>
//#include <string.h>


void ps()
{
	int i,j,count,position, proc_pid[50],k;
	char state[10];
	count=0;
	
	for(i=0; i<NPROC; i++)
	{
		if(proctab[i].pstate!=PRFREE)
		{
			//kprintf("\n\n PID : %d", i);
			if(count==0)
			{
				proc_pid[count++]=i;	//stores PID
				position=0;
				//proctab[proc_pid[0]].procCpuTicks-=1;
			}
			else
			{
				for(j=0; j<count; j++)
				{
					if(proctab[i].procCpuTicks > proctab[proc_pid[j]].procCpuTicks)
					{
						position = j;
						break;
					}
					else if(proctab[i].procCpuTicks == proctab[proc_pid[j]].procCpuTicks)
					{
						/*if CPU ticks are same, order by PID*/
						for(k=j; (proctab[i].procCpuTicks == proctab[proc_pid[k]].procCpuTicks); k++)
						{
							if( i > proc_pid[k])
							{
								position = k;
								break;
							}
							else
							{
								position = (k+1);
							}
						}
						break;
					}
					else
					{	
						position = count;
					}
				}
				for(j=count; j>=position; j--)
				{
					proc_pid[j] = proc_pid[j-1];
					//kprintf("\n\n proc PID = pos : %d, j ->  %d", proc_pid[j], j);
				}
				proc_pid[position] = i;
				

				count++;
			}
		}
	}


	kprintf("\n\n\n\tProcessName\t\tPID\t\tPriority\t\tStatus\t\tTicks\t\tStackSize");

	for(i=0; i<count; i++)
	{
		//kprintf("\n\n proc sorted: %d", proc_pid[i]);
		
			if(proctab[proc_pid[i]].pstate==PRCURR)
				strcpy(state,"PRCURR");
				
			else if(proctab[proc_pid[i]].pstate==PRREADY)
				strcpy(state,"PRREADY");
			
			else if(proctab[proc_pid[i]].pstate==PRRECV)
				strcpy(state,"PRRECV");
				
			else if(proctab[proc_pid[i]].pstate==PRSLEEP)
				strcpy(state,"PRSLEEP");
				
			else if(proctab[proc_pid[i]].pstate==PRSUSP)
				strcpy(state,"PRSUSP");
				
			else if(proctab[proc_pid[i]].pstate==PRWAIT)
				strcpy(state,"PRWAIT");
				
			else if(proctab[proc_pid[i]].pstate==PRTRECV)
				strcpy(state,"PRTRECV");
				
		kprintf("\n\t%s\t\t\t%d\t\t%d\t\t\t%s\t\t%d\t\t%d", proctab[proc_pid[i]].pname, proc_pid[i], proctab[proc_pid[i]].pprio, state, proctab[proc_pid[i]].procCpuTicks, proctab[proc_pid[i]].pstklen);
		
	}
}
