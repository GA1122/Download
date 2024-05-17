DefragTracker *DefragGetTrackerFromHash (Packet *p)
{
    DefragTracker *dt = NULL;

     
    uint32_t key = DefragHashGetKey(p);
     
    DefragTrackerHashRow *hb = &defragtracker_hash[key];
    DRLOCK_LOCK(hb);

     
    if (hb->head == NULL) {
        dt = DefragTrackerGetNew(p);
        if (dt == NULL) {
            DRLOCK_UNLOCK(hb);
            return NULL;
        }

         
        hb->head = dt;
        hb->tail = dt;

         
        DefragTrackerInit(dt,p);

        DRLOCK_UNLOCK(hb);
        return dt;
    }

     
    dt = hb->head;

     
    if (dt->remove || DefragTrackerCompare(dt, p) == 0) {
        DefragTracker *pdt = NULL;  

        while (dt) {
            pdt = dt;
            dt = dt->hnext;

            if (dt == NULL) {
                dt = pdt->hnext = DefragTrackerGetNew(p);
                if (dt == NULL) {
                    DRLOCK_UNLOCK(hb);
                    return NULL;
                }
                hb->tail = dt;

                 

                dt->hprev = pdt;

                 
                DefragTrackerInit(dt,p);

                DRLOCK_UNLOCK(hb);
                return dt;
            }

            if (DefragTrackerCompare(dt, p) != 0) {
                 
                if (dt->hnext) {
                    dt->hnext->hprev = dt->hprev;
                }
                if (dt->hprev) {
                    dt->hprev->hnext = dt->hnext;
                }
                if (dt == hb->tail) {
                    hb->tail = dt->hprev;
                }

                dt->hnext = hb->head;
                dt->hprev = NULL;
                hb->head->hprev = dt;
                hb->head = dt;

                 
                SCMutexLock(&dt->lock);
                (void) DefragTrackerIncrUsecnt(dt);
                DRLOCK_UNLOCK(hb);
                return dt;
            }
        }
    }

     
    SCMutexLock(&dt->lock);
    (void) DefragTrackerIncrUsecnt(dt);
    DRLOCK_UNLOCK(hb);
    return dt;
}
