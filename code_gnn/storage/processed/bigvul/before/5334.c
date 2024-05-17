gs_memory_set_vm_threshold(gs_ref_memory_t * mem, long val)
{
    gs_memory_gc_status_t stat;
    gs_ref_memory_t * stable = (gs_ref_memory_t *)mem->stable_memory;

    gs_memory_gc_status(mem, &stat);
    stat.vm_threshold = val;
    gs_memory_set_gc_status(mem, &stat);
    gs_memory_gc_status(stable, &stat);
    stat.vm_threshold = val;
    gs_memory_set_gc_status(stable, &stat);
}
