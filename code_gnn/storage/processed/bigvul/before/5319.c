alloc_trace_space(const gs_ref_memory_t *imem)
{
    return imem->space + (imem->stable_memory == (const gs_memory_t *)imem);
}
