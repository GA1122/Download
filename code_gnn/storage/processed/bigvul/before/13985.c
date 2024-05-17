gsicc_new_srcgtag_profile(gs_memory_t *memory)
{
    cmm_srcgtag_profile_t *result;
    int k;

    result = (cmm_srcgtag_profile_t *) gs_alloc_bytes(memory->non_gc_memory,
                                            sizeof(cmm_srcgtag_profile_t),
                                            "gsicc_new_srcgtag_profile");
    if (result == NULL)
        return NULL;
    result->memory = memory->non_gc_memory;

    for (k = 0; k < NUM_SOURCE_PROFILES; k++) {
        result->rgb_profiles[k] = NULL;
        result->cmyk_profiles[k] = NULL;
        result->gray_profiles[k] = NULL;
        result->gray_rend_cond[k].black_point_comp = gsBPNOTSPECIFIED;
        result->gray_rend_cond[k].rendering_intent = gsRINOTSPECIFIED;
        result->gray_rend_cond[k].override_icc = false;
        result->gray_rend_cond[k].preserve_black = gsBKPRESNOTSPECIFIED;
        result->gray_rend_cond[k].cmm = gsCMM_DEFAULT;
        result->rgb_rend_cond[k].black_point_comp = gsBPNOTSPECIFIED;
        result->rgb_rend_cond[k].rendering_intent = gsRINOTSPECIFIED;
        result->rgb_rend_cond[k].override_icc = false;
        result->rgb_rend_cond[k].preserve_black = gsBKPRESNOTSPECIFIED;
        result->rgb_rend_cond[k].cmm = gsCMM_DEFAULT;
        result->cmyk_rend_cond[k].black_point_comp = gsBPNOTSPECIFIED;
        result->cmyk_rend_cond[k].rendering_intent = gsRINOTSPECIFIED;
        result->cmyk_rend_cond[k].override_icc = false;
        result->cmyk_rend_cond[k].preserve_black = gsBKPRESNOTSPECIFIED;
        result->cmyk_rend_cond[k].cmm = gsCMM_DEFAULT;
    }
    result->color_warp_profile = NULL;
    result->name = NULL;
    result->name_length = 0;
    rc_init_free(result, memory->non_gc_memory, 1, rc_free_srcgtag_profile);
    return result;
}
