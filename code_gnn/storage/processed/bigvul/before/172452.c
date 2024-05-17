bool venc_dev::venc_set_color_format(OMX_COLOR_FORMATTYPE color_format)
{
 struct v4l2_format fmt;
 int color_space = 0;
    DEBUG_PRINT_LOW("venc_set_color_format: color_format = %u ", color_format);

 if ((int)color_format == (int)OMX_COLOR_FormatYUV420SemiPlanar ||
 (int)color_format == (int)QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m) {
        m_sVenc_cfg.inputformat = V4L2_PIX_FMT_NV12;
        color_space = V4L2_COLORSPACE_BT878;
 } else if ((int)color_format == (int)QOMX_COLOR_FormatYVU420SemiPlanar) {
        m_sVenc_cfg.inputformat = V4L2_PIX_FMT_NV21;
        color_space = V4L2_COLORSPACE_BT878;
 } else {
        DEBUG_PRINT_HIGH("WARNING: Unsupported Color format [%d]", color_format);
        m_sVenc_cfg.inputformat = V4L2_PIX_FMT_NV12;
        DEBUG_PRINT_HIGH("Default color format YUV420SemiPlanar is set");
 }

    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    fmt.fmt.pix_mp.pixelformat = m_sVenc_cfg.inputformat;
    fmt.fmt.pix_mp.colorspace = static_cast<decltype(fmt.fmt.pix_mp.colorspace)>(color_space);
    fmt.fmt.pix_mp.height = m_sVenc_cfg.input_height;
    fmt.fmt.pix_mp.width = m_sVenc_cfg.input_width;

 if (ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt)) {
        DEBUG_PRINT_ERROR("Failed setting color format %x", color_format);
 return false;
 }

 return true;
}
