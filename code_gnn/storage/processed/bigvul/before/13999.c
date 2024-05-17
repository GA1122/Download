gsicc_set_iccsmaskprofile(const char *pname,
                          int namelen, gsicc_manager_t *icc_manager,
                          gs_memory_t *mem)
{
    stream *str;
    int code;
    cmm_profile_t *icc_profile;

    if (icc_manager == NULL) {
        code = gsicc_open_search(pname, namelen, mem, NULL, 0, &str);
    } else {
        code = gsicc_open_search(pname, namelen, mem, mem->gs_lib_ctx->profiledir,
                                 mem->gs_lib_ctx->profiledir_len, &str);
    }
    if (code < 0 || str == NULL)
        return NULL;
    icc_profile = gsicc_profile_new(str, mem, pname, namelen);
    code = sfclose(str);
    if (icc_profile == NULL)
        return NULL;
     
    icc_profile->profile_handle =
            gsicc_get_profile_handle_buffer(icc_profile->buffer,
                                            icc_profile->buffer_size,
                                            mem);
     
    gsicc_get_icc_buff_hash(icc_profile->buffer, &(icc_profile->hashcode),
                            icc_profile->buffer_size);
    icc_profile->hash_is_valid = true;
    icc_profile->num_comps =
            gscms_get_input_channel_count(icc_profile->profile_handle);
    icc_profile->num_comps_out =
            gscms_get_output_channel_count(icc_profile->profile_handle);
    icc_profile->data_cs =
            gscms_get_profile_data_space(icc_profile->profile_handle);
    gscms_set_icc_range(&icc_profile);
    return icc_profile;
}
