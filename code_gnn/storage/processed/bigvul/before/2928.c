get_pdf14_device_proto(gx_device * dev, pdf14_device ** pdevproto,
        pdf14_device * ptempdevproto, gs_gstate * pgs,
        const gs_pdf14trans_t * pdf14pct, bool use_pdf14_accum)
{
    bool using_blend_cs;
    pdf14_default_colorspace_t dev_cs =
                pdf14_determine_default_blend_cs(dev, use_pdf14_accum,
                                                 &using_blend_cs);

    switch (dev_cs) {
        case PDF14_DeviceGray:
            *pdevproto = (pdf14_device *)&gs_pdf14_Gray_device;
            *ptempdevproto = **pdevproto;
            ptempdevproto->color_info.max_components = 1;
            ptempdevproto->color_info.num_components =
                                    ptempdevproto->color_info.max_components;
            ptempdevproto->color_info.max_gray = 255;
            ptempdevproto->color_info.gray_index = 0;  
            ptempdevproto->color_info.dither_grays = 256;
            ptempdevproto->sep_device = false;
            *pdevproto = ptempdevproto;
            break;
        case PDF14_DeviceRGB:
            *pdevproto = (pdf14_device *)&gs_pdf14_RGB_device;
            *ptempdevproto = **pdevproto;
            ptempdevproto->sep_device = false;
            *pdevproto = ptempdevproto;
            break;
        case PDF14_DeviceCMYK:
            *pdevproto = (pdf14_device *)&gs_pdf14_CMYK_device;
            *ptempdevproto = **pdevproto;
            ptempdevproto->sep_device = false;
            *pdevproto = ptempdevproto;
            break;
        case PDF14_DeviceCMYKspot:
            *pdevproto = (pdf14_device *)&gs_pdf14_CMYKspot_device;
             
             
            if (pdf14pct->params.num_spot_colors >= 0) {
                *ptempdevproto = **pdevproto;
                ptempdevproto->devn_params.page_spot_colors =
                    pdf14pct->params.num_spot_colors;
                ptempdevproto->color_info.num_components =
                    ptempdevproto->devn_params.num_std_colorant_names +
                    pdf14pct->params.num_spot_colors;
                if (ptempdevproto->color_info.num_components >
                        GS_CLIENT_COLOR_MAX_COMPONENTS)
                    ptempdevproto->color_info.num_components =
                        GS_CLIENT_COLOR_MAX_COMPONENTS;
                ptempdevproto->color_info.depth =
                                    ptempdevproto->color_info.num_components * 8;
                ptempdevproto->sep_device = true;
                *pdevproto = ptempdevproto;
            }
            break;
        case PDF14_DeviceCustom:
             
            *ptempdevproto = gs_pdf14_custom_device;
            ptempdevproto->color_info = dev->color_info;
             
            ptempdevproto->color_info.depth =
                ptempdevproto->color_info.num_components * 8;
            ptempdevproto->color_info.max_gray = 255;
            ptempdevproto->color_info.max_color = 255;
            ptempdevproto->color_info.dither_grays = 256;
            ptempdevproto->color_info.dither_colors = 256;

            *pdevproto = ptempdevproto;
            break;
        default:			 
            return_error(gs_error_rangecheck);
    }
    ptempdevproto->using_blend_cs = using_blend_cs;
    return 0;
}
