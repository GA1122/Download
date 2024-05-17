gs_heap_free_object(gs_memory_t * mem, void *ptr, client_name_t cname)
{
    gs_malloc_memory_t *mmem = (gs_malloc_memory_t *) mem;
    gs_malloc_block_t *bp;
    gs_memory_type_ptr_t pstype;
    struct_proc_finalize((*finalize));

    if_debug3m('a', mem, "[a-]gs_free(%s) 0x%lx(%u)\n",
               client_name_string(cname), (ulong) ptr,
               (ptr == 0 ? 0 : ((gs_malloc_block_t *) ptr)[-1].size));
    if (ptr == 0)
        return;
    pstype = ((gs_malloc_block_t *) ptr)[-1].type;
    finalize = pstype->finalize;
    if (finalize != 0) {
        if_debug3m('u', mem, "[u]finalizing %s 0x%lx (%s)\n",
                   struct_type_name_string(pstype),
                   (ulong) ptr, client_name_string(cname));
        (*finalize) (mem, ptr);
    }
    if (mmem->monitor)
        gx_monitor_enter(mmem->monitor);	 
     
#if 1
    bp = &((gs_malloc_block_t *)ptr)[-1];
    if (bp->prev)
        bp->prev->next = bp->next;
    if (bp->next)
        bp->next->prev = bp->prev;
    if (bp == mmem->allocated) {
        mmem->allocated = bp->next;
        mmem->allocated->prev = NULL;
    }
    mmem->used -= bp->size + sizeof(gs_malloc_block_t);
    if (mmem->monitor)
        gx_monitor_leave(mmem->monitor);	 
    gs_alloc_fill(bp, gs_alloc_fill_free,
                  bp->size + sizeof(gs_malloc_block_t));
    free(bp);
#else
    bp = mmem->allocated;  
    if (ptr == bp + 1) {
        mmem->allocated = bp->next;
        mmem->used -= bp->size + sizeof(gs_malloc_block_t);

        if (mmem->allocated)
            mmem->allocated->prev = 0;
        if (mmem->monitor)
            gx_monitor_leave(mmem->monitor);	 
        gs_alloc_fill(bp, gs_alloc_fill_free,
                      bp->size + sizeof(gs_malloc_block_t));
        free(bp);
    } else {
        gs_malloc_block_t *np;

         
        if (bp) {
            for (; (np = bp->next) != 0; bp = np) {
                if (ptr == np + 1) {
                    bp->next = np->next;
                    if (np->next)
                        np->next->prev = bp;
                    mmem->used -= np->size + sizeof(gs_malloc_block_t);
                    if (mmem->monitor)
                        gx_monitor_leave(mmem->monitor);	 
                    gs_alloc_fill(np, gs_alloc_fill_free,
                                  np->size + sizeof(gs_malloc_block_t));
                    free(np);
                    return;
                }
            }
        }
        if (mmem->monitor)
            gx_monitor_leave(mmem->monitor);	 
        lprintf2("%s: free 0x%lx not found!\n",
                 client_name_string(cname), (ulong) ptr);
        free((char *)((gs_malloc_block_t *) ptr - 1));
    }
#endif
}
