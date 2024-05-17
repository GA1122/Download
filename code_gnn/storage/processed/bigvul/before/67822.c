void DefragHashShutdown(void)
{
    DefragTracker *dt;
    uint32_t u;

    DefragTrackerPrintStats();

     
    while((dt = DefragTrackerDequeue(&defragtracker_spare_q))) {
        BUG_ON(SC_ATOMIC_GET(dt->use_cnt) > 0);
        DefragTrackerFree(dt);
    }

     
    if (defragtracker_hash != NULL) {
        for (u = 0; u < defrag_config.hash_size; u++) {
            dt = defragtracker_hash[u].head;
            while (dt) {
                DefragTracker *n = dt->hnext;
                DefragTrackerClearMemory(dt);
                DefragTrackerFree(dt);
                dt = n;
            }

            DRLOCK_DESTROY(&defragtracker_hash[u]);
        }
        SCFree(defragtracker_hash);
        defragtracker_hash = NULL;
    }
    (void) SC_ATOMIC_SUB(defrag_memuse, defrag_config.hash_size * sizeof(DefragTrackerHashRow));
    DefragTrackerQueueDestroy(&defragtracker_spare_q);

    SC_ATOMIC_DESTROY(defragtracker_prune_idx);
    SC_ATOMIC_DESTROY(defrag_memuse);
    SC_ATOMIC_DESTROY(defragtracker_counter);
    return;
}