gsicc_new_colorname(gs_memory_t *memory)
{
    gsicc_colorname_t *result;

    result = gs_alloc_struct(memory,gsicc_colorname_t,
                &st_gsicc_colorname, "gsicc_new_colorname");
    if (result == NULL)
        return NULL;
    result->length = 0;
    result->name = NULL;
    result->next = NULL;
    return result;
}
