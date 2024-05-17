bool venc_dev::venc_set_searchrange()
{
    DEBUG_PRINT_LOW("venc_set_searchrange");
 struct v4l2_control control;
 struct v4l2_ext_control ctrl[6];
 struct v4l2_ext_controls controls;
 int rc;

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_MPEG4) {
        ctrl[0].id = V4L2_CID_MPEG_VIDC_VIDEO_IFRAME_X_RANGE;
        ctrl[0].value = 16;
        ctrl[1].id = V4L2_CID_MPEG_VIDC_VIDEO_IFRAME_Y_RANGE;
        ctrl[1].value = 4;
        ctrl[2].id = V4L2_CID_MPEG_VIDC_VIDEO_PFRAME_X_RANGE;
        ctrl[2].value = 16;
        ctrl[3].id = V4L2_CID_MPEG_VIDC_VIDEO_PFRAME_Y_RANGE;
        ctrl[3].value = 4;
        ctrl[4].id = V4L2_CID_MPEG_VIDC_VIDEO_BFRAME_X_RANGE;
        ctrl[4].value = 12;
        ctrl[5].id = V4L2_CID_MPEG_VIDC_VIDEO_BFRAME_Y_RANGE;
        ctrl[5].value = 4;
 } else if ((m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) ||
 (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8)) {
        ctrl[0].id = V4L2_CID_MPEG_VIDC_VIDEO_IFRAME_X_RANGE;
        ctrl[0].value = 16;
        ctrl[1].id = V4L2_CID_MPEG_VIDC_VIDEO_IFRAME_Y_RANGE;
        ctrl[1].value = 4;
        ctrl[2].id = V4L2_CID_MPEG_VIDC_VIDEO_PFRAME_X_RANGE;
        ctrl[2].value = 16;
        ctrl[3].id = V4L2_CID_MPEG_VIDC_VIDEO_PFRAME_Y_RANGE;
        ctrl[3].value = 4;
        ctrl[4].id = V4L2_CID_MPEG_VIDC_VIDEO_BFRAME_X_RANGE;
        ctrl[4].value = 12;
        ctrl[5].id = V4L2_CID_MPEG_VIDC_VIDEO_BFRAME_Y_RANGE;
        ctrl[5].value = 4;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H263) {
        ctrl[0].id = V4L2_CID_MPEG_VIDC_VIDEO_IFRAME_X_RANGE;
        ctrl[0].value = 4;
        ctrl[1].id = V4L2_CID_MPEG_VIDC_VIDEO_IFRAME_Y_RANGE;
        ctrl[1].value = 4;
        ctrl[2].id = V4L2_CID_MPEG_VIDC_VIDEO_PFRAME_X_RANGE;
        ctrl[2].value = 4;
        ctrl[3].id = V4L2_CID_MPEG_VIDC_VIDEO_PFRAME_Y_RANGE;
        ctrl[3].value = 4;
        ctrl[4].id = V4L2_CID_MPEG_VIDC_VIDEO_BFRAME_X_RANGE;
        ctrl[4].value = 4;
        ctrl[5].id = V4L2_CID_MPEG_VIDC_VIDEO_BFRAME_Y_RANGE;
        ctrl[5].value = 4;
 } else {
        DEBUG_PRINT_ERROR("Invalid codec type");
 return false;
 }
    controls.count = 6;
    controls.ctrl_class = V4L2_CTRL_CLASS_MPEG;
    controls.controls = ctrl;

    DEBUG_PRINT_LOW(" Calling IOCTL set control for"
 "id=%x, val=%d id=%x, val=%d"
 "id=%x, val=%d id=%x, val=%d"
 "id=%x, val=%d id=%x, val=%d",
        controls.controls[0].id, controls.controls[0].value,
        controls.controls[1].id, controls.controls[1].value,
        controls.controls[2].id, controls.controls[2].value,
        controls.controls[3].id, controls.controls[3].value,
        controls.controls[4].id, controls.controls[4].value,
        controls.controls[5].id, controls.controls[5].value);

    rc = ioctl(m_nDriver_fd, VIDIOC_S_EXT_CTRLS, &controls);
 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set search range %d", rc);
 return false;
 }
 return true;
}
