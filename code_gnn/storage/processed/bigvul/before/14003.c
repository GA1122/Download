gsicc_smask_finalize(const gs_memory_t *memory, void * vptr)
{
    gsicc_smask_t *iccsmask = (gsicc_smask_t *)vptr;

    rc_decrement(iccsmask->smask_gray,
        "gsicc_smask_finalize");
    rc_decrement(iccsmask->smask_rgb,
        "gsicc_smask_finalize");
    rc_decrement(iccsmask->smask_cmyk,
        "gsicc_smask_finalize");
}
