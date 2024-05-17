gsicc_set_device_profile(gx_device * pdev, gs_memory_t * mem,
                         char *file_name, gsicc_profile_types_t pro_enum)
{
    cmm_profile_t *icc_profile;
    stream *str;
    int code;

     
    while(pdev->child)
        pdev = pdev->child;

     
    if (file_name != '\0') {
         
        if (strncmp(file_name, OI_PROFILE, strlen(OI_PROFILE)) == 0)
            return -1;

        code = gsicc_open_search(file_name, strlen(file_name), mem,
                                 mem->gs_lib_ctx->profiledir,
                                 mem->gs_lib_ctx->profiledir_len, &str);
        if (code < 0)
            return code;
        if (str != NULL) {
            icc_profile =
                gsicc_profile_new(str, mem, file_name, strlen(file_name));
            code = sfclose(str);
            if (icc_profile == NULL)
                return gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
            if (pro_enum < gsPROOFPROFILE) {
                if_debug1m(gs_debug_flag_icc, mem,
                           "[icc] Setting device profile %d\n", pro_enum);
                pdev->icc_struct->device_profile[pro_enum] = icc_profile;
            } else {
                 
                if (pro_enum == gsPROOFPROFILE) {
                    if_debug0m(gs_debug_flag_icc, mem, "[icc] Setting proof profile\n");
                    pdev->icc_struct->proof_profile = icc_profile;
                } else if (pro_enum ==  gsLINKPROFILE) {
                    if_debug0m(gs_debug_flag_icc, mem, "[icc] Setting link profile\n");
                    pdev->icc_struct->link_profile = icc_profile;
                } else {
                    if_debug0m(gs_debug_flag_icc, mem, "[icc] Setting postrender profile\n");
                    pdev->icc_struct->postren_profile = icc_profile;
                }
            }
             
            icc_profile->profile_handle =
                gsicc_get_profile_handle_buffer(icc_profile->buffer,
                                                icc_profile->buffer_size,
                                                mem);
            if (icc_profile->profile_handle == NULL)
                return_error(gs_error_unknownerror);
             
            gsicc_get_icc_buff_hash(icc_profile->buffer,
                                    &(icc_profile->hashcode),
                                    icc_profile->buffer_size);
            icc_profile->hash_is_valid = true;
             
            icc_profile->num_comps =
                gscms_get_input_channel_count(icc_profile->profile_handle);
            if_debug1m(gs_debug_flag_icc, mem, "[icc] Profile has %d components\n",
                       icc_profile->num_comps);
            icc_profile->num_comps_out =
                gscms_get_output_channel_count(icc_profile->profile_handle);
            icc_profile->data_cs =
                gscms_get_profile_data_space(icc_profile->profile_handle);
             
            switch (icc_profile->num_comps) {
                case 1:
                    if (strncmp(icc_profile->name, DEFAULT_GRAY_ICC,
                    strlen(icc_profile->name)) == 0) {
                        icc_profile->default_match = DEFAULT_GRAY;
                    }
                    break;
                case 3:
                    if (strncmp(icc_profile->name, DEFAULT_RGB_ICC,
                    strlen(icc_profile->name)) == 0) {
                        icc_profile->default_match = DEFAULT_RGB;
                    }
                    break;
                case 4:
                    if (strncmp(icc_profile->name, DEFAULT_CMYK_ICC,
                    strlen(icc_profile->name)) == 0) {
                        icc_profile->default_match = DEFAULT_CMYK;
                    }
                    break;
                default:
                     
                    gsicc_set_device_profile_colorants(pdev, NULL);
                    break;
            }
            if_debug1m(gs_debug_flag_icc, mem, "[icc] Profile data CS is %d\n",
                       icc_profile->data_cs);
        } else
            return gs_rethrow(-1, "cannot find device profile");
    }
    return 0;
}
