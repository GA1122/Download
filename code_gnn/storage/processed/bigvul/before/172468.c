bool venc_dev::venc_set_ltrmode(OMX_U32 enable, OMX_U32 count)
{
    DEBUG_PRINT_LOW("venc_set_ltrmode: enable = %u", (unsigned int)enable);
 struct v4l2_control control;
 struct v4l2_ext_control ctrl[2];
 struct v4l2_ext_controls controls;
 int rc;

 if (!venc_validate_hybridhp_params(0, 0, count, 0)) {
        DEBUG_PRINT_ERROR("Invalid settings, LTR enabled with HybridHP");
 return false;
 }

    ctrl[0].id = V4L2_CID_MPEG_VIDC_VIDEO_LTRMODE;
 if (enable)
        ctrl[0].value = V4L2_MPEG_VIDC_VIDEO_LTR_MODE_MANUAL;
 else
        ctrl[0].value = V4L2_MPEG_VIDC_VIDEO_LTR_MODE_DISABLE;

    ctrl[1].id = V4L2_CID_MPEG_VIDC_VIDEO_LTRCOUNT;
 if (enable && count > 0)
        ctrl[1].value = count;
 else if (enable)
        ctrl[1].value = 1;
 else
        ctrl[1].value = 0;

    controls.count = 2;
    controls.ctrl_class = V4L2_CTRL_CLASS_MPEG;
    controls.controls = ctrl;

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%x, val=%d id=%x, val=%d",
                    controls.controls[0].id, controls.controls[0].value,
                    controls.controls[1].id, controls.controls[1].value);

    rc = ioctl(m_nDriver_fd, VIDIOC_S_EXT_CTRLS, &controls);
 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set ltrmode %d", rc);
 return false;
 }
    ltrinfo.enabled = enable;
    ltrinfo.count = count;

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%x, val=%d id=%x, val=%d",
                    controls.controls[0].id, controls.controls[0].value,
                    controls.controls[1].id, controls.controls[1].value);

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
    control.value = V4L2_MPEG_VIDC_EXTRADATA_LTR;

 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
        DEBUG_PRINT_ERROR("ERROR: Request for setting extradata failed");
 return false;
 }

 if (!venc_set_profile_level(0, 0)) {
        DEBUG_PRINT_ERROR("ERROR: %s(): Driver Profile/Level is NOT SET",
                __func__);
 } else {
        DEBUG_PRINT_HIGH("%s(): Driver Profile[%lu]/Level[%lu] successfully SET",
                __func__, codec_profile.profile, profile_level.level);
 }

 return true;
}
