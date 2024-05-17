gsicc_new_device_profile_array(gs_memory_t *memory)
{
    cmm_dev_profile_t *result;
    int k;

    if_debug0m(gs_debug_flag_icc,memory,"[icc] Allocating device profile struct\n");
    result = (cmm_dev_profile_t *) gs_alloc_bytes(memory->non_gc_memory,
                                            sizeof(cmm_dev_profile_t),
                                            "gsicc_new_device_profile_array");
    if (result == NULL)
        return NULL;
    result->memory = memory->non_gc_memory;

    for (k = 0; k < NUM_DEVICE_PROFILES; k++) {
        result->device_profile[k] = NULL;
        result->rendercond[k].rendering_intent = gsRINOTSPECIFIED;
        result->rendercond[k].black_point_comp = gsBPNOTSPECIFIED;
        result->rendercond[k].override_icc = false;
        result->rendercond[k].preserve_black = gsBKPRESNOTSPECIFIED;
        result->rendercond[k].graphics_type_tag = GS_UNKNOWN_TAG;
        result->rendercond[k].cmm = gsCMM_DEFAULT;
    }
    result->proof_profile = NULL;
    result->link_profile = NULL;
    result->postren_profile = NULL;
    result->oi_profile = NULL;
    result->spotnames = NULL;
    result->devicegraytok = true;   
    result->graydetection = false;
    result->pageneutralcolor = false;
    result->usefastcolor = false;   
    result->prebandthreshold = true;
    result->supports_devn = false;
    result->sim_overprint = false;   
    rc_init_free(result, memory->non_gc_memory, 1, rc_free_profile_array);
    return result;
}
