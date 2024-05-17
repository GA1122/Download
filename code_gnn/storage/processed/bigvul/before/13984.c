gsicc_new_iccsmask(gs_memory_t *memory)
{
    gsicc_smask_t *result;

    result = (gsicc_smask_t *) gs_alloc_struct(memory, gsicc_smask_t, &st_gsicc_smask, "gsicc_new_iccsmask");
    if (result != NULL) {
        result->smask_gray = NULL;
        result->smask_rgb = NULL;
        result->smask_cmyk = NULL;
        result->memory = memory;
        result->swapped = false;
    }
    return result;
}
