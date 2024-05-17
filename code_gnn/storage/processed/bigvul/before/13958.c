gsicc_get_default_type(cmm_profile_t *profile_data)
{
    switch (profile_data->default_match) {
        case DEFAULT_GRAY:
            return gs_color_space_index_DeviceGray;
        case DEFAULT_RGB:
            return gs_color_space_index_DeviceRGB;
        case DEFAULT_CMYK:
            return gs_color_space_index_DeviceCMYK;
        case CIE_A:
            return gs_color_space_index_CIEA;
        case CIE_ABC:
            return gs_color_space_index_CIEABC;
        case CIE_DEF:
            return gs_color_space_index_CIEDEF;
        case CIE_DEFG:
            return gs_color_space_index_CIEDEFG;
        default:
            return gs_color_space_index_ICC;
    }
}
