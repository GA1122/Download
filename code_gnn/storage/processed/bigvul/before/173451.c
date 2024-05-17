bool omx_vdec::allocate_color_convert_buf::get_color_format(OMX_COLOR_FORMATTYPE &dest_color_format)
{
 bool status = true;
 if (!enabled) {
 if (omx->drv_ctx.output_format == VDEC_YUV_FORMAT_NV12)
 if (omx->drv_ctx.decoder_format == VDEC_CODECTYPE_MVC)
                    dest_color_format = (OMX_COLOR_FORMATTYPE)
                        QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mMultiView;
 else
            dest_color_format = (OMX_COLOR_FORMATTYPE)
                QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m;
 else
            status = false;
 } else {
 if (ColorFormat == OMX_COLOR_FormatYUV420Planar ||
 ColorFormat == OMX_COLOR_FormatYUV420SemiPlanar) {
            dest_color_format = ColorFormat;
 } else
            status = false;
 }
 return status;
}
