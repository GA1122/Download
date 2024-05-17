bool venc_dev::venc_enable_initial_qp(QOMX_EXTNINDEX_VIDEO_INITIALQP* initqp)
{
 int rc;
 struct v4l2_control control;
 struct v4l2_ext_control ctrl[4];
 struct v4l2_ext_controls controls;

    ctrl[0].id = V4L2_CID_MPEG_VIDC_VIDEO_I_FRAME_QP;
    ctrl[0].value = initqp->nQpI;
    ctrl[1].id = V4L2_CID_MPEG_VIDC_VIDEO_P_FRAME_QP;
    ctrl[1].value = initqp->nQpP;
    ctrl[2].id = V4L2_CID_MPEG_VIDC_VIDEO_B_FRAME_QP;
    ctrl[2].value = initqp->nQpB;
    ctrl[3].id = V4L2_CID_MPEG_VIDC_VIDEO_ENABLE_INITIAL_QP;
    ctrl[3].value = initqp->bEnableInitQp;

    controls.count = 4;
    controls.ctrl_class = V4L2_CTRL_CLASS_MPEG;
    controls.controls = ctrl;

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%x val=%d, id=%x val=%d, id=%x val=%d, id=%x val=%d",
                    controls.controls[0].id, controls.controls[0].value,
                    controls.controls[1].id, controls.controls[1].value,
                    controls.controls[2].id, controls.controls[2].value,
                    controls.controls[3].id, controls.controls[3].value);

    rc = ioctl(m_nDriver_fd, VIDIOC_S_EXT_CTRLS, &controls);
 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set session_qp %d", rc);
 return false;
 }

    init_qp.iframeqp = initqp->nQpI;
    init_qp.pframeqp = initqp->nQpP;
    init_qp.bframeqp = initqp->nQpB;
    init_qp.enableinitqp = initqp->bEnableInitQp;

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%x val=%d, id=%x val=%d, id=%x val=%d, id=%x val=%d",
                    controls.controls[0].id, controls.controls[0].value,
                    controls.controls[1].id, controls.controls[1].value,
                    controls.controls[2].id, controls.controls[2].value,
                    controls.controls[3].id, controls.controls[3].value);
 return true;
}
