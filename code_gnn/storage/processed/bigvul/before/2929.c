gs_pdf14_device_copy_params(gx_device *dev, const gx_device *target)
{
    cmm_dev_profile_t *profile_targ;
    cmm_dev_profile_t *profile_dev14;
    pdf14_device *pdev = (pdf14_device*) dev;

    COPY_PARAM(width);
    COPY_PARAM(height);
    COPY_ARRAY_PARAM(MediaSize);
    COPY_ARRAY_PARAM(ImagingBBox);
    COPY_PARAM(ImagingBBox_set);
    COPY_ARRAY_PARAM(HWResolution);
    COPY_ARRAY_PARAM(Margins);
    COPY_ARRAY_PARAM(HWMargins);
    COPY_PARAM(PageCount);
    COPY_PARAM(MaxPatternBitmap);
    COPY_PARAM(graphics_type_tag);
    COPY_PARAM(interpolate_control);
    memcpy(&(dev->space_params), &(target->space_params), sizeof(gdev_space_params));
     
    if (dev->icc_struct == NULL) {
        dev->icc_struct = gsicc_new_device_profile_array(dev->memory);
        profile_dev14 = dev->icc_struct;
        dev_proc((gx_device *) target, get_profile)((gx_device *) target,
                                          &(profile_targ));
        gsicc_adjust_profile_rc(profile_targ->device_profile[0], 1, "gs_pdf14_device_copy_params");
        if (profile_dev14->device_profile[0] != NULL) {
            gsicc_adjust_profile_rc(profile_dev14->device_profile[0], -1, "gs_pdf14_device_copy_params");
        }
        profile_dev14->device_profile[0] = profile_targ->device_profile[0];
        dev->icc_struct->devicegraytok = profile_targ->devicegraytok;
        dev->icc_struct->graydetection = profile_targ->graydetection;
        dev->icc_struct->pageneutralcolor = profile_targ->pageneutralcolor;
        dev->icc_struct->supports_devn = profile_targ->supports_devn;
        dev->icc_struct->usefastcolor = profile_targ->usefastcolor;
        profile_dev14->rendercond[0] = profile_targ->rendercond[0];
        if (pdev->using_blend_cs) {
             
            gsicc_adjust_profile_rc(profile_targ->device_profile[0], 1, "gs_pdf14_device_copy_params");
            gsicc_adjust_profile_rc(profile_targ->blend_profile, 1, "gs_pdf14_device_copy_params");
            gsicc_adjust_profile_rc(profile_dev14->device_profile[0], -1, "gs_pdf14_device_copy_params");
            gsicc_adjust_profile_rc(profile_dev14->blend_profile, -1, "gs_pdf14_device_copy_params");
            profile_dev14->blend_profile = profile_targ->device_profile[0];
            profile_dev14->device_profile[0] = profile_targ->blend_profile;
        }
        profile_dev14->sim_overprint = profile_targ->sim_overprint;
    }
#undef COPY_ARRAY_PARAM
#undef COPY_PARAM
}
