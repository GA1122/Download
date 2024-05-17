rc_free_profile_array(gs_memory_t * mem, void *ptr_in, client_name_t cname)
{
    cmm_dev_profile_t *icc_struct = (cmm_dev_profile_t *)ptr_in;
    int k;
    gs_memory_t *mem_nongc =  icc_struct->memory;

    if (icc_struct->rc.ref_count <= 1 ) {
         
        for (k = 0; k < NUM_DEVICE_PROFILES; k++) {
            if (icc_struct->device_profile[k] != NULL) {
                if_debug1m(gs_debug_flag_icc, mem_nongc,
                           "[icc] Releasing device profile %d\n", k);
                rc_decrement(icc_struct->device_profile[k],
                             "rc_free_profile_array");
            }
        }
        if (icc_struct->link_profile != NULL) {
            if_debug0m(gs_debug_flag_icc,mem_nongc,"[icc] Releasing link profile\n");
            rc_decrement(icc_struct->link_profile, "rc_free_profile_array");
        }
        if (icc_struct->proof_profile != NULL) {
            if_debug0m(gs_debug_flag_icc,mem_nongc,"[icc] Releasing proof profile\n");
            rc_decrement(icc_struct->proof_profile, "rc_free_profile_array");
        }
        if (icc_struct->oi_profile != NULL) {
            if_debug0m(gs_debug_flag_icc,mem_nongc,"[icc] Releasing oi profile\n");
            rc_decrement(icc_struct->oi_profile, "rc_free_profile_array");
        }
        if (icc_struct->postren_profile != NULL) {
            if_debug0m(gs_debug_flag_icc, mem_nongc, "[icc] Releasing postren profile\n");
            rc_decrement(icc_struct->postren_profile, "rc_free_profile_array");
        }
        if (icc_struct->spotnames != NULL) {
            if_debug0m(gs_debug_flag_icc, mem_nongc, "[icc] Releasing spotnames\n");
             
            gsicc_free_spotnames(icc_struct->spotnames, mem_nongc);
             
            gs_free_object(mem_nongc, icc_struct->spotnames, "rc_free_profile_array");
        }
        if_debug0m(gs_debug_flag_icc,mem_nongc,"[icc] Releasing device profile struct\n");
        gs_free_object(mem_nongc, icc_struct, "rc_free_profile_array");
    }
}
