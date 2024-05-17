gsicc_initialize_iccsmask(gsicc_manager_t *icc_manager)
{
    gs_memory_t *stable_mem = icc_manager->memory->stable_memory;

     
    icc_manager->smask_profiles = gsicc_new_iccsmask(stable_mem);
    if (icc_manager->smask_profiles == NULL)
        return gs_throw(gs_error_VMerror, "insufficient memory to allocate smask profiles");
     
    if ((icc_manager->smask_profiles->smask_gray =
        gsicc_set_iccsmaskprofile(SMASK_GRAY_ICC, strlen(SMASK_GRAY_ICC),
        icc_manager, stable_mem) ) == NULL) {
        return gs_throw(-1, "failed to load gray smask profile");
    }
    if ((icc_manager->smask_profiles->smask_rgb =
        gsicc_set_iccsmaskprofile(SMASK_RGB_ICC, strlen(SMASK_RGB_ICC),
        icc_manager, stable_mem)) == NULL) {
        return gs_throw(-1, "failed to load rgb smask profile");
    }
    if ((icc_manager->smask_profiles->smask_cmyk =
        gsicc_set_iccsmaskprofile(SMASK_CMYK_ICC, strlen(SMASK_CMYK_ICC),
        icc_manager, stable_mem)) == NULL) {
        return gs_throw(-1, "failed to load cmyk smask profile");
    }
     
    icc_manager->smask_profiles->smask_gray->default_match = DEFAULT_GRAY;
    icc_manager->smask_profiles->smask_rgb->default_match = DEFAULT_RGB;
    icc_manager->smask_profiles->smask_cmyk->default_match = DEFAULT_CMYK;
    return 0;
}
