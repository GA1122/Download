bool venc_dev::venc_set_session_qp(OMX_U32 i_frame_qp, OMX_U32 p_frame_qp,OMX_U32 b_frame_qp)
{
 int rc;
 struct v4l2_control control;

    control.id = V4L2_CID_MPEG_VIDEO_H264_I_FRAME_QP;
    control.value = i_frame_qp;

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);
    session_qp.iframeqp = control.value;

    control.id = V4L2_CID_MPEG_VIDEO_H264_P_FRAME_QP;
    control.value = p_frame_qp;

    DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
    rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

    DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);

    session_qp.pframeqp = control.value;

 if ((codec_profile.profile == V4L2_MPEG_VIDEO_H264_PROFILE_MAIN) ||
 (codec_profile.profile == V4L2_MPEG_VIDEO_H264_PROFILE_HIGH)) {

        control.id = V4L2_CID_MPEG_VIDEO_H264_B_FRAME_QP;
        control.value = b_frame_qp;

        DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);

        session_qp.bframeqp = control.value;
 }

 return true;
}
