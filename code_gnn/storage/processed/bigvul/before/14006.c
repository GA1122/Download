rc_free_srcgtag_profile(gs_memory_t * mem, void *ptr_in, client_name_t cname)
{
    cmm_srcgtag_profile_t *srcgtag_profile = (cmm_srcgtag_profile_t *)ptr_in;
    int k;
    gs_memory_t *mem_nongc =  srcgtag_profile->memory;

    if (srcgtag_profile->rc.ref_count <= 1 ) {
         
        for (k = 0; k < NUM_SOURCE_PROFILES; k++) {
            if (srcgtag_profile->gray_profiles[k] != NULL) {
                rc_decrement(srcgtag_profile->gray_profiles[k],
                    "rc_free_srcgtag_profile(gray)");
            }
            if (srcgtag_profile->rgb_profiles[k] != NULL) {
                rc_decrement(srcgtag_profile->rgb_profiles[k],
                             "rc_free_srcgtag_profile(rgb)");
            }
            if (srcgtag_profile->cmyk_profiles[k] != NULL) {
                rc_decrement(srcgtag_profile->cmyk_profiles[k],
                             "rc_free_srcgtag_profile(cmyk)");
            }
            if (srcgtag_profile->color_warp_profile != NULL) {
                rc_decrement(srcgtag_profile->color_warp_profile,
                             "rc_free_srcgtag_profile(warp)");
            }
        }
        gs_free_object(mem_nongc, srcgtag_profile->name, "rc_free_srcgtag_profile");
        gs_free_object(mem_nongc, srcgtag_profile, "rc_free_srcgtag_profile");
    }
}
