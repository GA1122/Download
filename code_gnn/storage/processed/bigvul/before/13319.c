pdf14_increment_smask_color(gs_gstate * pgs, gx_device * dev)
{
    pdf14_device * pdev = (pdf14_device *) dev;
    pdf14_smaskcolor_t *result;
    gsicc_smask_t *smask_profiles = pgs->icc_manager->smask_profiles;
    int k;

     
    if (pdev->smaskcolor == NULL && pgs->icc_manager->smask_profiles != NULL &&
        pgs->icc_manager->smask_profiles->swapped) {
            return 0;
    }
    if (pdev->smaskcolor != NULL) {
        pdev->smaskcolor->ref_count++;
        if_debug1m(gs_debug_flag_icc, dev->memory,
                   "[icc] Increment smask color now %d\n",
                   pdev->smaskcolor->ref_count);
    } else {
         
        result = gs_alloc_struct(pdev->memory, pdf14_smaskcolor_t,
                                &st_pdf14_smaskcolor,
                                "pdf14_increment_smask_color");
        if (result == NULL ) return(-1);
        result->profiles = gsicc_new_iccsmask(pdev->memory);
        if (result->profiles == NULL ) return(-1);
        pdev->smaskcolor = result;
         
        result->profiles->smask_gray = pgs->icc_manager->default_gray;
        result->profiles->smask_rgb = pgs->icc_manager->default_rgb;
        result->profiles->smask_cmyk = pgs->icc_manager->default_cmyk;
        pgs->icc_manager->default_gray = smask_profiles->smask_gray;
        pgs->icc_manager->default_rgb = smask_profiles->smask_rgb;
        pgs->icc_manager->default_cmyk = smask_profiles->smask_cmyk;
        pgs->icc_manager->smask_profiles->swapped = true;
        if_debug0m(gs_debug_flag_icc, pgs->memory,
                   "[icc] Initial creation of smask color. Ref count 1\n");
        pdev->smaskcolor->ref_count = 1;
         
        for (k = 0; k < 2; k++) {
            gs_color_space *pcs     = pgs->color[k].color_space;
            cmm_profile_t  *profile = pcs->cmm_icc_profile_data;
            if (profile != NULL) {
                switch(profile->data_cs) {
                    case gsGRAY:
                        if (profile->hashcode ==
                            result->profiles->smask_gray->hashcode) {
                                profile = pgs->icc_manager->default_gray;
                        }
                        break;
                    case gsRGB:
                        if (profile->hashcode ==
                            result->profiles->smask_rgb->hashcode) {
                                profile = pgs->icc_manager->default_rgb;
                        }
                        break;
                    case gsCMYK:
                        if (profile->hashcode ==
                            result->profiles->smask_cmyk->hashcode) {
                                profile = pgs->icc_manager->default_cmyk;
                        }
                        break;
                    default:

                        break;
                }
                rc_assign(pcs->cmm_icc_profile_data, profile,
                          "pdf14_increment_smask_color");
            }
        }
    }
    return(0);
}
