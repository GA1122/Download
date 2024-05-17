seticc_lab(i_ctx_t * i_ctx_p, float *white, float *black, float *range_buff)
{
    int                     code;
    gs_color_space *        pcs;
    int                     i;

     
    code = gs_cspace_build_ICC(&pcs, NULL, gs_gstate_memory(igs));
    if (code < 0)
        return gs_rethrow(code, "building color space object");
     
     
    if (igs->icc_manager->lab_profile == NULL ) {
         
        return gs_rethrow(code, "cannot find lab icc profile");
    }
     
    code = gsicc_set_gscs_profile(pcs, igs->icc_manager->lab_profile, gs_gstate_memory(igs));
    if (code < 0)
        return gs_rethrow(code, "installing the lab profile");
    pcs->cmm_icc_profile_data->Range.ranges[0].rmin = 0.0;
    pcs->cmm_icc_profile_data->Range.ranges[0].rmax = 100.0;
    for (i = 1; i < 3; i++) {
        pcs->cmm_icc_profile_data->Range.ranges[i].rmin =
            range_buff[2 * (i-1)];
        pcs->cmm_icc_profile_data->Range.ranges[i].rmax =
            range_buff[2 * (i-1) + 1];
    }
     
    code = gs_setcolorspace(igs, pcs);
    return code;
}
