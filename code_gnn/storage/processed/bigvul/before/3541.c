gs_heap_free_all(gs_memory_t * mem, uint free_mask, client_name_t cname)
{
    gs_malloc_memory_t *const mmem = (gs_malloc_memory_t *) mem;
    gx_monitor_t *mon = mmem->monitor;

     
    mmem->monitor = NULL; 	 
    gx_monitor_free(mon);	 
#ifndef MEMENTO
     
    if (free_mask & FREE_ALL_DATA) {
        gs_malloc_block_t *bp = mmem->allocated;
        gs_malloc_block_t *np;

        for (; bp != 0; bp = np) {
            np = bp->next;
            if_debug3m('a', mem, "[a]gs_heap_free_all(%s) 0x%lx(%u)\n",
                       client_name_string(bp->cname), (ulong) (bp + 1),
                       bp->size);
            gs_alloc_fill(bp + 1, gs_alloc_fill_free, bp->size);
            free(bp);
        }
    }
#endif
    if (free_mask & FREE_ALL_ALLOCATOR)
        free(mem);
}
