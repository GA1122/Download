bool omx_vdec::allocate_color_convert_buf::set_color_format(
        OMX_COLOR_FORMATTYPE dest_color_format)
{
 bool status = true;
    OMX_COLOR_FORMATTYPE drv_color_format;
 if (!omx) {
        DEBUG_PRINT_ERROR("Invalid client in color convert");
 return false;
 }
    pthread_mutex_lock(&omx->c_lock);
 if (omx->drv_ctx.output_format == VDEC_YUV_FORMAT_NV12)
 if (omx->drv_ctx.decoder_format == VDEC_CODECTYPE_MVC)
            drv_color_format = (OMX_COLOR_FORMATTYPE)
                QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mMultiView;
 else
        drv_color_format = (OMX_COLOR_FORMATTYPE)
            QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m;
 else {
        DEBUG_PRINT_ERROR("Incorrect color format");
        status = false;
 }
 if (status && !omx->is_component_secure() &&
        drv_color_format != dest_color_format &&
        drv_color_format != (OMX_COLOR_FORMATTYPE)
                QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mMultiView) {
        DEBUG_PRINT_LOW("Enabling C2D");
 if ((dest_color_format != OMX_COLOR_FormatYUV420Planar) &&
 (dest_color_format != OMX_COLOR_FormatYUV420SemiPlanar)) {
            DEBUG_PRINT_ERROR("Unsupported color format for c2d");
            status = false;
 } else {
 ColorFormat = dest_color_format;
            dest_format = (dest_color_format == OMX_COLOR_FormatYUV420Planar) ?
 YCbCr420P : YCbCr420SP;
 if (enabled)
                c2d.destroy();
            enabled = false;
 if (!c2d.init()) {
                DEBUG_PRINT_ERROR("open failed for c2d");
                status = false;
 } else
                enabled = true;
 }
 } else {
 if (enabled)
            c2d.destroy();
        enabled = false;
 }
    pthread_mutex_unlock(&omx->c_lock);
 return status;
}
