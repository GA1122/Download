void omx_vdec::fill_aspect_ratio_info(
 struct vdec_aspectratioinfo *aspect_ratio_info,
        OMX_QCOM_EXTRADATA_FRAMEINFO *frame_info)
{
    m_extradata = frame_info;
    m_extradata->aspectRatio.aspectRatioX = aspect_ratio_info->par_width;
    m_extradata->aspectRatio.aspectRatioY = aspect_ratio_info->par_height;
    DEBUG_PRINT_LOW("aspectRatioX %u aspectRatioY %u", (unsigned int)m_extradata->aspectRatio.aspectRatioX,
 (unsigned int)m_extradata->aspectRatio.aspectRatioY);
}
