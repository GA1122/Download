static RELOC_PTRS_WITH(ref_memory_reloc_ptrs, gs_ref_memory_t *mptr)
{
    RELOC_PTR(gs_ref_memory_t, streams);
    RELOC_PTR(gs_ref_memory_t, names_array);
    RELOC_PTR(gs_ref_memory_t, changes);
    RELOC_PTR(gs_ref_memory_t, scan_limit);
     
    mptr->reloc_saved = RELOC_OBJ(mptr->saved);
}
