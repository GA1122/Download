static DefragTracker *DefragTrackerGetUsedDefragTracker(void)
{
    uint32_t idx = SC_ATOMIC_GET(defragtracker_prune_idx) % defrag_config.hash_size;
    uint32_t cnt = defrag_config.hash_size;

    while (cnt--) {
        if (++idx >= defrag_config.hash_size)
            idx = 0;

        DefragTrackerHashRow *hb = &defragtracker_hash[idx];

        if (DRLOCK_TRYLOCK(hb) != 0)
            continue;

        DefragTracker *dt = hb->tail;
        if (dt == NULL) {
            DRLOCK_UNLOCK(hb);
            continue;
        }

        if (SCMutexTrylock(&dt->lock) != 0) {
            DRLOCK_UNLOCK(hb);
            continue;
        }

         
        if (SC_ATOMIC_GET(dt->use_cnt) > 0) {
            DRLOCK_UNLOCK(hb);
            SCMutexUnlock(&dt->lock);
            continue;
        }

         
        if (dt->hprev != NULL)
            dt->hprev->hnext = dt->hnext;
        if (dt->hnext != NULL)
            dt->hnext->hprev = dt->hprev;
        if (hb->head == dt)
            hb->head = dt->hnext;
        if (hb->tail == dt)
            hb->tail = dt->hprev;

        dt->hnext = NULL;
        dt->hprev = NULL;
        DRLOCK_UNLOCK(hb);

        DefragTrackerClearMemory(dt);

        SCMutexUnlock(&dt->lock);

        (void) SC_ATOMIC_ADD(defragtracker_prune_idx, (defrag_config.hash_size - cnt));
        return dt;
    }

    return NULL;
}