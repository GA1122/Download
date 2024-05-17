pdf14_ok_to_optimize(gx_device *dev)
{
    bool using_blend_cs;
    pdf14_default_colorspace_t pdf14_cs =
        pdf14_determine_default_blend_cs(dev, false, &using_blend_cs);
    gsicc_colorbuffer_t dev_icc_cs;
    bool ok = false;
    int tag_depth = (dev->graphics_type_tag & GS_DEVICE_ENCODES_TAGS) ? 8 : 0;
    cmm_dev_profile_t *dev_profile;
    int code = dev_proc(dev, get_profile)(dev,  &dev_profile);

    if (code < 0)
        return false;
    dev_icc_cs = dev_profile->device_profile[0]->data_cs;
     
     
    if (!(dev_icc_cs == gsGRAY || dev_icc_cs == gsRGB || dev_icc_cs == gsCMYK) || using_blend_cs)
        return false;                            

    switch (pdf14_cs) {
        case PDF14_DeviceGray:
            ok = dev->color_info.max_gray == 255 && dev->color_info.depth == 8 + tag_depth;
            break;
        case PDF14_DeviceRGB:
            ok = dev->color_info.max_color == 255 && dev->color_info.depth == 24 + tag_depth;
            break;
        case PDF14_DeviceCMYK:
            ok = dev->color_info.max_color == 255 && dev->color_info.depth == 32 + tag_depth;
            break;
        case PDF14_DeviceCMYKspot:
            ok = false;			 
            break;
        case PDF14_DeviceCustom:
             
            ok = dev->color_info.depth == dev->color_info.num_components * 8 + tag_depth;
            break;
        default:			 
            ok = false;
    }
    return ok;
}
