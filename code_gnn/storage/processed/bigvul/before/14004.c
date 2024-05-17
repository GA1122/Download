rc_free_icc_profile(gs_memory_t * mem, void *ptr_in, client_name_t cname)
{
    cmm_profile_t *profile = (cmm_profile_t *)ptr_in;
    gs_memory_t *mem_nongc =  profile->memory;

    if_debug2m(gs_debug_flag_icc, mem,
               "[icc] rc decrement profile = 0x%p rc = %ld\n",
               ptr_in, profile->rc.ref_count);
    if (profile->rc.ref_count <= 1 ) {
         
        if (profile->buffer != NULL) {
            gs_free_object(mem_nongc, profile->buffer, "rc_free_icc_profile(buffer)");
            profile->buffer = NULL;
        }
        if_debug0m(gs_debug_flag_icc, mem, "[icc] profile freed\n");
         
        if (profile->profile_handle != NULL) {
            profile->release(profile->profile_handle);
            profile->profile_handle = NULL;
        }
         
        if (profile->name != NULL) {
            gs_free_object(mem_nongc, profile->name,"rc_free_icc_profile(name)");
            profile->name = NULL;
            profile->name_length = 0;
        }
        profile->hash_is_valid = 0;
        if (profile->lock != NULL) {
            gx_monitor_free(profile->lock);
            profile->lock = NULL;
        }
         
        if (profile->spotnames != NULL) {
             
            gsicc_free_spotnames(profile->spotnames, mem_nongc);
             
            gs_free_object(mem_nongc, profile->spotnames, "rc_free_icc_profile(spotnames)");
        }
         
        if (profile->v2_data != NULL) {
            gs_free_object(mem_nongc, profile->v2_data, "rc_free_icc_profile(v2_data)");
        }
        gs_free_object(mem_nongc, profile, "rc_free_icc_profile");
    }
}
