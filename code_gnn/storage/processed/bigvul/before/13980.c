gsicc_manager_new(gs_memory_t *memory)
{
    gsicc_manager_t *result;

     
    result = gs_alloc_struct(memory->stable_memory, gsicc_manager_t, &st_gsicc_manager,
                             "gsicc_manager_new");
    if ( result == NULL )
        return NULL;
    rc_init_free(result, memory->stable_memory, 1, rc_gsicc_manager_free);
    result->default_gray = NULL;
    result->default_rgb = NULL;
    result->default_cmyk = NULL;
    result->lab_profile = NULL;
    result->xyz_profile = NULL;
    result->graytok_profile = NULL;
    result->device_named = NULL;
    result->device_n = NULL;
    result->smask_profiles = NULL;
    result->memory = memory->stable_memory;
    result->srcgtag_profile = NULL;
    result->override_internal = false;
    return result;
}
