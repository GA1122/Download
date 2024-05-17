bool venc_dev::venc_set_hybrid_hierp(OMX_U32 layers)
{
    DEBUG_PRINT_LOW("venc_set_hybrid_hierp layers: %u", layers);
 struct v4l2_control control;
 int rc;

 if (!venc_validate_hybridhp_params(layers, 0, 0, (int) HIER_P_HYBRID)) {
        DEBUG_PRINT_ERROR("Invalid settings, Hybrid HP enabled with LTR OR Hier-pLayers OR bframes");
 return false;
 }

 if (!layers || layers > MAX_HYB_HIERP_LAYERS) {
        DEBUG_PRINT_ERROR("Invalid numbers of layers set: %d (max supported is 6)", layers);
 return false;
 }

    hier_layers.numlayers = layers;
    hier_layers.hier_mode = HIER_P_HYBRID;
 if (venc_calibrate_gop()) {
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_NUM_P_FRAMES;
        control.value = intra_period.num_pframes;
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        control.id = V4L2_CID_MPEG_VIDC_VIDEO_NUM_B_FRAMES;
        control.value = intra_period.num_bframes;
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }
        DEBUG_PRINT_LOW("Updated nPframes (%ld) and nBframes (%ld)",
                         intra_period.num_pframes, intra_period.num_bframes);
 } else {
        DEBUG_PRINT_ERROR("Invalid settings, Hybrid HP enabled with LTR OR Hier-pLayers OR bframes");
 return false;
 }

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_HYBRID_HIERP_MODE;
    control.value = layers - 1;

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%x, val=%d",
                    control.id, control.value);

    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set hybrid hierp %d", rc);
 return false;
 }

    DEBUG_PRINT_LOW("SUCCESS IOCTL set control for id=%x, val=%d",
                    control.id, control.value);
    control.id = V4L2_CID_MPEG_VIDC_VIDEO_H264_NAL_SVC;
    control.value = V4L2_CID_MPEG_VIDC_VIDEO_H264_NAL_SVC_ENABLED;
 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
        DEBUG_PRINT_ERROR("Failed to enable SVC_NAL");
 return false;
 }
 return true;
}
