gsicc_set_default_cs_value(cmm_profile_t *picc_profile, gs_gstate *pgs)
{
    gsicc_manager_t *icc_manager = pgs->icc_manager;
    int64_t hashcode = picc_profile->hashcode;

    if ( picc_profile->default_match == DEFAULT_NONE ) {
        switch ( picc_profile->data_cs ) {
            case gsGRAY:
                if ( hashcode == icc_manager->default_gray->hashcode )
                    picc_profile->default_match = DEFAULT_GRAY_s;
                break;
            case gsRGB:
                if ( hashcode == icc_manager->default_rgb->hashcode )
                    picc_profile->default_match = DEFAULT_RGB_s;
                break;
            case gsCMYK:
                if ( hashcode == icc_manager->default_cmyk->hashcode )
                    picc_profile->default_match = DEFAULT_CMYK_s;
                break;
            case gsCIELAB:
                if ( hashcode == icc_manager->lab_profile->hashcode )
                    picc_profile->default_match = LAB_TYPE_s;
                break;
            case gsCIEXYZ:
                return;
                break;
            case gsNCHANNEL:
                return;
                break;
            default:
                return;
        }
    }
}
