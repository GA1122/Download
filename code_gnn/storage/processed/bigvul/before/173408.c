void omx_vdec::append_frame_info_extradata(OMX_OTHER_EXTRADATATYPE *extra,
        OMX_U32 num_conceal_mb, OMX_U32 picture_type, OMX_U32 frame_rate,
        OMX_TICKS time_stamp, struct msm_vidc_panscan_window_payload *panscan_payload,
 struct vdec_aspectratioinfo *aspect_ratio_info)
{
    OMX_QCOM_EXTRADATA_FRAMEINFO *frame_info = NULL;
 struct msm_vidc_panscan_window *panscan_window;
 if (!(client_extradata & OMX_FRAMEINFO_EXTRADATA)) {
 return;
 }
    extra->nSize = OMX_FRAMEINFO_EXTRADATA_SIZE;
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataFrameInfo;
    extra->nDataSize = sizeof(OMX_QCOM_EXTRADATA_FRAMEINFO);
    frame_info = (OMX_QCOM_EXTRADATA_FRAMEINFO *)(void *)extra->data;
 switch (picture_type) {
 case PICTURE_TYPE_I:
            frame_info->ePicType = OMX_VIDEO_PictureTypeI;
 break;
 case PICTURE_TYPE_P:
            frame_info->ePicType = OMX_VIDEO_PictureTypeP;
 break;
 case PICTURE_TYPE_B:
            frame_info->ePicType = OMX_VIDEO_PictureTypeB;
 break;
 default:
            frame_info->ePicType = (OMX_VIDEO_PICTURETYPE)0;
 }
 if (drv_ctx.interlace == VDEC_InterlaceInterleaveFrameTopFieldFirst)
        frame_info->interlaceType = OMX_QCOM_InterlaceInterleaveFrameTopFieldFirst;
 else if (drv_ctx.interlace == VDEC_InterlaceInterleaveFrameBottomFieldFirst)
        frame_info->interlaceType = OMX_QCOM_InterlaceInterleaveFrameBottomFieldFirst;
 else
        frame_info->interlaceType = OMX_QCOM_InterlaceFrameProgressive;
    memset(&frame_info->aspectRatio, 0, sizeof(frame_info->aspectRatio));
    frame_info->nConcealedMacroblocks = num_conceal_mb;
    frame_info->nFrameRate = frame_rate;
    frame_info->nTimeStamp = time_stamp;
    frame_info->panScan.numWindows = 0;
 if (output_capability == V4L2_PIX_FMT_MPEG2) {
 if (m_disp_hor_size && m_disp_vert_size) {
            frame_info->displayAspectRatio.displayHorizontalSize = m_disp_hor_size;
            frame_info->displayAspectRatio.displayVerticalSize = m_disp_vert_size;
 } else {
            frame_info->displayAspectRatio.displayHorizontalSize = 0;
            frame_info->displayAspectRatio.displayVerticalSize = 0;
 }
 }

 if (panscan_payload) {
        frame_info->panScan.numWindows = panscan_payload->num_panscan_windows;
        panscan_window = &panscan_payload->wnd[0];
 for (OMX_U32 i = 0; i < frame_info->panScan.numWindows; i++) {
            frame_info->panScan.window[i].x = panscan_window->panscan_window_width;
            frame_info->panScan.window[i].y = panscan_window->panscan_window_height;
            frame_info->panScan.window[i].dx = panscan_window->panscan_width_offset;
            frame_info->panScan.window[i].dy = panscan_window->panscan_height_offset;
            panscan_window++;
 }
 }
    fill_aspect_ratio_info(aspect_ratio_info, frame_info);
    print_debug_extradata(extra);
}
