gsicc_init_device_profile_struct(gx_device * dev,
                                 char *profile_name,
                                 gsicc_profile_types_t profile_type)
{
    int code;
    cmm_profile_t *curr_profile;
    cmm_dev_profile_t *profile_struct;

     
    profile_struct = dev->icc_struct;
    if (profile_struct != NULL) {
         
        if (profile_type < gsPROOFPROFILE) {
            curr_profile = profile_struct->device_profile[profile_type];
        } else {
             
            if (profile_type == gsPROOFPROFILE) {
                curr_profile = profile_struct->proof_profile;
            } else if (profile_type == gsLINKPROFILE) {
                curr_profile = profile_struct->link_profile;
            } else {
                curr_profile = profile_struct->postren_profile;
            }
        }
         
        if (curr_profile != NULL) {
             
            if (profile_name != NULL) {
                if (strncmp(curr_profile->name, profile_name,
                            strlen(profile_name)) != 0 &&
                    strncmp(curr_profile->name, OI_PROFILE,
                            strlen(curr_profile->name)) != 0) {
                     
                    rc_decrement(curr_profile, "gsicc_init_device_profile_struct");
                } else {
                     
                    return 0;
                }
            }
        }
    } else {
         
        dev->icc_struct = gsicc_new_device_profile_array(dev->memory);
        profile_struct = dev->icc_struct;
        if (profile_struct == NULL)
            return_error(gs_error_VMerror);
    }
     
    if (profile_name == NULL) {
        profile_name =
            (char *) gs_alloc_bytes(dev->memory,
                                    MAX_DEFAULT_ICC_LENGTH,
                                    "gsicc_init_device_profile_struct");
        if (profile_name == NULL)
            return_error(gs_error_VMerror);
        switch(dev->color_info.num_components) {
            case 1:
                strncpy(profile_name, DEFAULT_GRAY_ICC, strlen(DEFAULT_GRAY_ICC));
                profile_name[strlen(DEFAULT_GRAY_ICC)] = 0;
                break;
            case 3:
                strncpy(profile_name, DEFAULT_RGB_ICC, strlen(DEFAULT_RGB_ICC));
                profile_name[strlen(DEFAULT_RGB_ICC)] = 0;
                break;
            case 4:
                strncpy(profile_name, DEFAULT_CMYK_ICC, strlen(DEFAULT_CMYK_ICC));
                profile_name[strlen(DEFAULT_CMYK_ICC)] = 0;
                break;
            default:
                strncpy(profile_name, DEFAULT_CMYK_ICC, strlen(DEFAULT_CMYK_ICC));
                profile_name[strlen(DEFAULT_CMYK_ICC)] = 0;
                break;
        }
         
        code = gsicc_set_device_profile(dev, dev->memory, profile_name,
                                        profile_type);
        gs_free_object(dev->memory, profile_name,
                       "gsicc_init_device_profile_struct");
        return code;
    } else {
         
        code = gsicc_set_device_profile(dev, dev->memory, profile_name,
                                        profile_type);
        return code;
    }
}
