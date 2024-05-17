gs_memory_set_vm_reclaim(gs_ref_memory_t * mem, bool enabled)
{
    gs_memory_gc_status_t stat;
    gs_ref_memory_t * stable = (gs_ref_memory_t *)mem->stable_memory;

    gs_memory_gc_status(mem, &stat);
    stat.enabled = enabled;
    gs_memory_set_gc_status(mem, &stat);
    gs_memory_gc_status(stable, &stat);
    stat.enabled = enabled;
    gs_memory_set_gc_status(stable, &stat);
}
