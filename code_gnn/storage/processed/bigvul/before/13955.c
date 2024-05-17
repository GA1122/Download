gsicc_fill_srcgtag_item(gsicc_rendering_param_t *r_params, char **pstrlast, bool cmyk)
{
    char *curr_ptr;
    int blackptcomp;
    int or_icc, preserve_k;
    int ri;

     
    curr_ptr = gs_strtok(NULL, "\t,\32\n\r", pstrlast);
    if (sscanf(curr_ptr, "%d", &ri) != 1)
        return_error(gs_error_unknownerror);
    r_params->rendering_intent = ri | gsRI_OVERRIDE;
     
    curr_ptr = gs_strtok(NULL, "\t,\32\n\r", pstrlast);
    if (sscanf(curr_ptr, "%d", &blackptcomp) != 1)
        return_error(gs_error_unknownerror);
    r_params->black_point_comp = blackptcomp | gsBP_OVERRIDE;
     
    curr_ptr = gs_strtok(NULL, "\t,\32\n\r", pstrlast);
    if (sscanf(curr_ptr, "%d", &or_icc) != 1)
        return_error(gs_error_unknownerror);
    r_params->override_icc = or_icc;
    if (cmyk) {
         
        curr_ptr = gs_strtok(NULL, "\t,\32\n\r", pstrlast);
        if (sscanf(curr_ptr, "%d", &preserve_k) < 1)
            return_error(gs_error_unknownerror);
        r_params->preserve_black = preserve_k | gsKP_OVERRIDE;
    } else {
        r_params->preserve_black = gsBKPRESNOTSPECIFIED;
    }
    return 0;
}
