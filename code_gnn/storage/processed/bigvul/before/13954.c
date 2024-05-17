gsicc_extract_profile(gs_graphics_type_tag_t graphics_type_tag,
                       cmm_dev_profile_t *profile_struct,
                       cmm_profile_t **profile, gsicc_rendering_param_t *render_cond)
{
    switch (graphics_type_tag & ~GS_DEVICE_ENCODES_TAGS) {
        case GS_UNKNOWN_TAG:
        case GS_UNTOUCHED_TAG:
        default:
            (*profile) = profile_struct->device_profile[0];
            *render_cond = profile_struct->rendercond[0];
            break;
        case GS_PATH_TAG:
            *render_cond = profile_struct->rendercond[1];
            if (profile_struct->device_profile[1] != NULL) {
                (*profile) = profile_struct->device_profile[1];
            } else {
                (*profile) = profile_struct->device_profile[0];
            }
            break;
        case GS_IMAGE_TAG:
            *render_cond = profile_struct->rendercond[2];
            if (profile_struct->device_profile[2] != NULL) {
                (*profile) = profile_struct->device_profile[2];
            } else {
                (*profile) = profile_struct->device_profile[0];
            }
            break;
        case GS_TEXT_TAG:
            *render_cond = profile_struct->rendercond[3];
            if (profile_struct->device_profile[3] != NULL) {
                (*profile) = profile_struct->device_profile[3];
            } else {
                (*profile) = profile_struct->device_profile[0];
            }
            break;
        }
}
