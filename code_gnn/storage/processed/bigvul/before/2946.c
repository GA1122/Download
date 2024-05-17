pdf14_determine_default_blend_cs(gx_device * pdev, bool use_pdf14_accum,
                                 bool *using_blend_cs)
{
     
    cmm_dev_profile_t *dev_profile;
    int code = dev_proc(pdev, get_profile)(pdev, &dev_profile);
    bool valid_blend_cs = false;
    *using_blend_cs = false;

     
    if (code == 0 && dev_profile->blend_profile != NULL && !use_pdf14_accum) {
        if (!dev_profile->blend_profile->isdevlink &&
            !dev_profile->blend_profile->islab &&
            (dev_profile->blend_profile->data_cs == gsGRAY ||
             dev_profile->blend_profile->data_cs == gsRGB ||
             dev_profile->blend_profile->data_cs == gsCMYK)) {
             
            if (!(gx_device_is_pattern_clist(pdev) ||
                  gx_device_is_pattern_accum(pdev))) {
                valid_blend_cs = true;
            }
        }
    }

     
    if (pdev->color_info.polarity == GX_CINFO_POLARITY_ADDITIVE ||
        pdev->color_info.num_components == 1) {
         
        if (valid_blend_cs) {
            *using_blend_cs = true;
            switch (dev_profile->blend_profile->num_comps) {
            case 1:
                return PDF14_DeviceGray;
            case 3:
                return PDF14_DeviceRGB;
            case 4:
                return PDF14_DeviceCMYK;
            }
        }
        if (pdev->color_info.num_components == 1)
            return PDF14_DeviceGray;
        else
            return PDF14_DeviceRGB;
    } else {
         
        int i, output_comp_num, num_cmyk_used = 0, num_cmyk = 0;
#if CUSTOM_BLENDING_MODE == ALWAYS_USE_CUSTOM_BLENDING
        return PDF14_DeviceCustom;
#endif
         
        for (i = 0; i < 4; i++) {
            const char * pcomp_name = (const char *)DeviceCMYKComponents[i];

            output_comp_num = dev_proc(pdev, get_color_comp_index)
                (pdev, pcomp_name, strlen(pcomp_name), NO_COMP_NAME_TYPE);
            if (output_comp_num >= 0) {
                num_cmyk++;
                if (output_comp_num != GX_DEVICE_COLOR_MAX_COMPONENTS)
                    num_cmyk_used++;
            }
        }
         
        if (num_cmyk_used == 4 && pdev->color_info.num_components == 4
            && pdev->color_info.max_components == 4) {
            if (valid_blend_cs) {
                *using_blend_cs = true;
                switch (dev_profile->blend_profile->num_comps) {
                case 1:
                    return PDF14_DeviceGray;
                case 3:
                    return PDF14_DeviceRGB;
                case 4:
                    return PDF14_DeviceCMYK;
                }
            }
            return PDF14_DeviceCMYK;
        }
         
#if CUSTOM_BLENDING_MODE == AUTO_USE_CUSTOM_BLENDING
        if (num_cmyk != 4)
            return PDF14_DeviceCustom;
#endif
         
        return PDF14_DeviceCMYKspot;
    }
}
