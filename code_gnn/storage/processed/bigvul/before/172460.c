bool venc_dev::venc_set_hier_layers(QOMX_VIDEO_HIERARCHICALCODINGTYPE type,
                                    OMX_U32 num_layers)
{
 struct v4l2_control control;

 if (!venc_validate_hybridhp_params(num_layers, 0, 0, (int)type)){
        DEBUG_PRINT_ERROR("Invalid settings, Hier-pLayers enabled with HybridHP");
 return false;
 }

 if (type == QOMX_HIERARCHICALCODING_P) {
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_HIER_P_NUM_LAYERS;
        control.value = num_layers - 1;
        DEBUG_PRINT_HIGH("Set Hier P num layers: %u", (unsigned int)num_layers);
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
            DEBUG_PRINT_ERROR("Request to set Hier P num layers failed");
 return false;
 }
 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
            DEBUG_PRINT_LOW("Set H264_SVC_NAL");
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_H264_NAL_SVC;
            control.value = V4L2_CID_MPEG_VIDC_VIDEO_H264_NAL_SVC_ENABLED;
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_ERROR("Failed to enable SVC_NAL");
 return false;
 }
 }
        hier_layers.hier_mode = HIER_P;
 } else if (type == QOMX_HIERARCHICALCODING_B) {
 if (m_sVenc_cfg.codectype != V4L2_PIX_FMT_HEVC) {
            DEBUG_PRINT_ERROR("Failed : Hier B layers supported only for HEVC encode");
 return false;
 }
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_HIER_B_NUM_LAYERS;
        control.value = num_layers - 1;
        DEBUG_PRINT_INFO("Set Hier B num layers: %u", (unsigned int)num_layers);
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
            DEBUG_PRINT_ERROR("Request to set Hier P num layers failed");
 return false;
 }
        hier_layers.hier_mode = HIER_B;
 } else {
        DEBUG_PRINT_ERROR("Request to set hier num layers failed for type: %d", type);
 return false;
 }
    hier_layers.numlayers = num_layers;
 return true;
}
