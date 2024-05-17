DefragTracker *DefragLookupTrackerFromHash (Packet *p)
{
    DefragTracker *dt = NULL;

     
    uint32_t key = DefragHashGetKey(p);
     
    DefragTrackerHashRow *hb = &defragtracker_hash[key];
    DRLOCK_LOCK(hb);

     
    if (hb->head == NULL) {
        DRLOCK_UNLOCK(hb);
        return dt;
    }

     
    dt = hb->head;

     
    if (DefragTrackerCompare(dt, p) == 0) {
        while (dt) {
            dt = dt->hnext;

            if (dt == NULL) {
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
