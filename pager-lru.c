#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;
    int currentProc;
    int desiredPage;
    long pc;

    // used to determine the oldest page out of all of the processes
    int oldestPage;
    int oldestTime;
    int currentTime;

    /* initialize static vars on first run */
    if (!initialized) {
        for (proctmp = 0; proctmp < MAXPROCESSES; proctmp++) {
            for (pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                timestamps[proctmp][pagetmp] = 0;
            }
        }
        initialized = 1;
    }

    for(currentProc = 0; currentProc < MAXPROCESSES; currentProc ++){  // for every process
        if(q[currentProc].active) {
            pc = q[currentProc].pc;
            desiredPage = (int)pc / PAGESIZE;

            if(!q[currentProc].pages[desiredPage]){  // if page isn't in memory
                if(!pagein(currentProc, desiredPage)) {  // attempt to add the page
                    oldestPage = -1;
                    oldestTime = -1;

                    // find least recently used page to replace
                    for(pagetmp = 0; pagetmp < q[currentProc].npages; pagetmp++){
                        // dont check the page we're trying to pagein
                        // also check that page @ pagetmp is paged in
                        if(pagetmp != desiredPage && q[currentProc].pages[pagetmp]){
                            currentTime = tick - timestamps[currentProc][pagetmp];

                            if(currentTime > oldestTime){
                                oldestPage = pagetmp;
                                oldestTime = currentTime;
                            }
                        }
                    }

                    // remove oldest page from process
                    pageout(currentProc, oldestPage);
                    timestamps[currentProc][desiredPage] = tick;

                    /* advance time for next pageit iteration */
                    tick++;
                }
            }
        }
    }
}
