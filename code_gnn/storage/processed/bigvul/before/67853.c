DefragTrackerFreeFrags(DefragTracker *tracker)
{
    Frag *frag;

     
    SCMutexLock(&defrag_context->frag_pool_lock);

    while ((frag = TAILQ_FIRST(&tracker->frags)) != NULL) {
        TAILQ_REMOVE(&tracker->frags, frag, next);

         
        DefragFragReset(frag);
        PoolReturn(defrag_context->frag_pool, frag);
    }

    SCMutexUnlock(&defrag_context->frag_pool_lock);
}
