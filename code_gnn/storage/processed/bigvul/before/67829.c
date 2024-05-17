static DefragTracker *DefragTrackerGetNew(Packet *p)
{
    DefragTracker *dt = NULL;

     
    dt = DefragTrackerDequeue(&defragtracker_spare_q);
    if (dt == NULL) {
         
        if (!(DEFRAG_CHECK_MEMCAP(sizeof(DefragTracker)))) {
             

                 

            dt = DefragTrackerGetUsedDefragTracker();
            if (dt == NULL) {
                return NULL;
            }

             
        } else {
             
            dt = DefragTrackerAlloc();
            if (dt == NULL) {
                return NULL;
            }

             
        }
    } else {
         

         
    }

    (void) SC_ATOMIC_ADD(defragtracker_counter, 1);
    SCMutexLock(&dt->lock);
    return dt;
}
