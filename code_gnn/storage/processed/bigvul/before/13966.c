gsicc_get_srcprofile(gsicc_colorbuffer_t data_cs,
                     gs_graphics_type_tag_t graphics_type_tag,
                     cmm_srcgtag_profile_t *srcgtag_profile,
                     cmm_profile_t **profile, gsicc_rendering_param_t *render_cond)
{
    (*profile) = NULL;
    (*render_cond).rendering_intent = gsPERCEPTUAL;
    switch (graphics_type_tag & ~GS_DEVICE_ENCODES_TAGS) {
        case GS_UNKNOWN_TAG:
        case GS_UNTOUCHED_TAG:
        default:
            break;
        case GS_PATH_TAG:
            if (data_cs == gsRGB) {
                (*profile) = srcgtag_profile->rgb_profiles[gsSRC_GRAPPRO];
                *render_cond = srcgtag_profile->rgb_rend_cond[gsSRC_GRAPPRO];
            } else if (data_cs == gsCMYK) {
                (*profile) = srcgtag_profile->cmyk_profiles[gsSRC_GRAPPRO];
                *render_cond = srcgtag_profile->cmyk_rend_cond[gsSRC_GRAPPRO];
            } else if (data_cs == gsGRAY) {
                (*profile) = srcgtag_profile->gray_profiles[gsSRC_GRAPPRO];
                *render_cond = srcgtag_profile->gray_rend_cond[gsSRC_GRAPPRO];
            }
            break;
        case GS_IMAGE_TAG:
            if (data_cs == gsRGB) {
                (*profile) = srcgtag_profile->rgb_profiles[gsSRC_IMAGPRO];
                *render_cond = srcgtag_profile->rgb_rend_cond[gsSRC_IMAGPRO];
            } else if (data_cs == gsCMYK) {
                (*profile) = srcgtag_profile->cmyk_profiles[gsSRC_IMAGPRO];
                *render_cond = srcgtag_profile->cmyk_rend_cond[gsSRC_IMAGPRO];
            } else if (data_cs == gsGRAY) {
                (*profile) = srcgtag_profile->gray_profiles[gsSRC_IMAGPRO];
                *render_cond = srcgtag_profile->gray_rend_cond[gsSRC_IMAGPRO];
            }
            break;
        case GS_TEXT_TAG:
            if (data_cs == gsRGB) {
                (*profile) = srcgtag_profile->rgb_profiles[gsSRC_TEXTPRO];
                *render_cond = srcgtag_profile->rgb_rend_cond[gsSRC_TEXTPRO];
            } else if (data_cs == gsCMYK) {
                (*profile) = srcgtag_profile->cmyk_profiles[gsSRC_TEXTPRO];
                *render_cond = srcgtag_profile->cmyk_rend_cond[gsSRC_TEXTPRO];
            } else if (data_cs == gsGRAY) {
                (*profile) = srcgtag_profile->gray_profiles[gsSRC_TEXTPRO];
                *render_cond = srcgtag_profile->gray_rend_cond[gsSRC_TEXTPRO];
            }
            break;
        }
}
