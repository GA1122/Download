gsicc_initialize_default_profile(cmm_profile_t *icc_profile)
{
    gsicc_profile_t defaulttype = icc_profile->default_match;
    gsicc_colorbuffer_t default_space = gsUNDEFINED;
    int num_comps, num_comps_out;
    gs_memory_t *mem = icc_profile->memory;

     
    if (icc_profile->profile_handle == NULL) {
        icc_profile->profile_handle =
                        gsicc_get_profile_handle_buffer(icc_profile->buffer,
                                                        icc_profile->buffer_size,
                                                        mem);
        if (icc_profile->profile_handle == NULL) {
            return gs_rethrow1(gs_error_VMerror, "allocation of profile %s handle failed",
                               icc_profile->name);
        }
    }
    if (icc_profile->buffer != NULL && icc_profile->hash_is_valid == false) {
         
        gsicc_get_icc_buff_hash(icc_profile->buffer, &(icc_profile->hashcode),
                                icc_profile->buffer_size);
        icc_profile->hash_is_valid = true;
    }
    num_comps = icc_profile->num_comps;
    icc_profile->num_comps =
        gscms_get_input_channel_count(icc_profile->profile_handle);
    num_comps_out = icc_profile->num_comps_out;
    icc_profile->num_comps_out =
        gscms_get_output_channel_count(icc_profile->profile_handle);
    icc_profile->data_cs =
        gscms_get_profile_data_space(icc_profile->profile_handle);
    if_debug0m(gs_debug_flag_icc,mem,"[icc] Setting ICC profile in Manager\n");
    switch(defaulttype) {
        case DEFAULT_GRAY:
            if_debug0m(gs_debug_flag_icc,mem,"[icc] Default Gray\n");
            default_space = gsGRAY;
            break;
        case DEFAULT_RGB:
            if_debug0m(gs_debug_flag_icc,mem,"[icc] Default RGB\n");
            default_space = gsRGB;
            break;
        case DEFAULT_CMYK:
            if_debug0m(gs_debug_flag_icc,mem,"[icc] Default CMYK\n");
            default_space = gsCMYK;
             break;
        case NAMED_TYPE:
            if_debug0m(gs_debug_flag_icc,mem,"[icc] Named Color\n");
            break;
        case LAB_TYPE:
            if_debug0m(gs_debug_flag_icc,mem,"[icc] CIELAB Profile\n");
            break;
        case DEVICEN_TYPE:
            if_debug0m(gs_debug_flag_icc,mem,"[icc] DeviceN Profile\n");
            break;
        case DEFAULT_NONE:
        default:
            return 0;
            break;
    }
    if_debug1m(gs_debug_flag_icc,mem,"[icc] name = %s\n", icc_profile->name);
    if_debug1m(gs_debug_flag_icc,mem,"[icc] num_comps = %d\n", icc_profile->num_comps);
     
    if (default_space != gsUNDEFINED ||
        num_comps != icc_profile->num_comps ||
        num_comps_out != icc_profile->num_comps_out) {
        if (icc_profile->data_cs != default_space) {
            return gs_rethrow(-1, "A default profile has an incorrect color space");
        }
    }
    return 0;
}
