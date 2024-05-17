seticc_cal(i_ctx_t * i_ctx_p, float *white, float *black, float *gamma,
           float *matrix, int num_colorants, ulong dictkey)
{
    int                     code;
    gs_color_space *        pcs;
    gs_memory_t             *mem = igs->memory;
    int                     i;
    cmm_profile_t           *cal_profile;

     
    pcs = gsicc_find_cs(dictkey, igs);
    if (pcs == NULL ) {
         
        code = gs_cspace_build_ICC(&pcs, NULL, mem->stable_memory);
        if (code < 0)
            return gs_rethrow(code, "building color space object");
         
        pcs->base_space = NULL;
         
        cal_profile = gsicc_create_from_cal(white, black, gamma, matrix,
                                            mem->stable_memory, num_colorants);
        if (cal_profile == NULL)
            return gs_rethrow(gs_error_VMerror, "creating the cal profile failed");
         
        code = gsicc_set_gscs_profile(pcs, cal_profile, mem->stable_memory);
         
        rc_decrement(cal_profile, "seticc_cal");
        if (code < 0)
            return gs_rethrow(code, "installing the cal profile");
        for (i = 0; i < num_colorants; i++) {
            pcs->cmm_icc_profile_data->Range.ranges[i].rmin = 0;
            pcs->cmm_icc_profile_data->Range.ranges[i].rmax = 1;
        }
         
        gsicc_add_cs(igs, pcs,dictkey);
    }
     
    code = gs_setcolorspace(igs, pcs);
    return code;
}
