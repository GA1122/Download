gs_malloc_memory_init(void)
{
    gs_malloc_memory_t *mem =
        (gs_malloc_memory_t *)Memento_label(malloc(sizeof(gs_malloc_memory_t)), "gs_malloc_memory_t");

    if (mem == NULL)
        return NULL;

    mem->stable_memory = 0;	 
    mem->procs = gs_malloc_memory_procs;
    mem->allocated = 0;
    mem->limit = max_long;
    mem->used = 0;
    mem->max_used = 0;
    mem->gs_lib_ctx = 0;
    mem->non_gc_memory = (gs_memory_t *)mem;
    mem->thread_safe_memory = (gs_memory_t *)mem;	 
     
    mem->monitor = NULL;	 
    mem->monitor = gx_monitor_alloc((gs_memory_t *)mem);

    return mem;
}
