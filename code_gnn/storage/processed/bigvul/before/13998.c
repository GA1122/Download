gsicc_set_gscs_profile(gs_color_space *pcs, cmm_profile_t *icc_profile,
                       gs_memory_t * mem)
{
    if (pcs == NULL)
        return -1;
#if ICC_DUMP
    if (icc_profile->buffer) {
        dump_icc_buffer(icc_profile->buffer_size, "set_gscs",
                        icc_profile->buffer);
        global_icc_index++;
    }
#endif

    rc_increment(icc_profile);
    if (pcs->cmm_icc_profile_data != NULL) {
         
         
         
        rc_decrement(pcs->cmm_icc_profile_data, "gsicc_set_gscs_profile");
    }
    pcs->cmm_icc_profile_data = icc_profile;
    return 0;
}
