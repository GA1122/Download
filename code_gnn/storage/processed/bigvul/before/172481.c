bool venc_dev::venc_set_session_qp_range(OMX_U32 min_qp, OMX_U32 max_qp)
{
 int rc;
 struct v4l2_control control;

 if ((min_qp >= session_qp_range.minqp) && (max_qp <= session_qp_range.maxqp)) {

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8)
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_VP8_MIN_QP;
 else
            control.id = V4L2_CID_MPEG_VIDEO_H264_MIN_QP;
        control.value = min_qp;

        DEBUG_PRINT_LOW("Calling IOCTL set MIN_QP control id=%d, val=%d",
                control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8)
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_VP8_MAX_QP;
 else
            control.id = V4L2_CID_MPEG_VIDEO_H264_MAX_QP;
        control.value = max_qp;

        DEBUG_PRINT_LOW("Calling IOCTL set MAX_QP control id=%d, val=%d",
                control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }
 } else {
        DEBUG_PRINT_ERROR("Wrong qp values[%u %u], allowed range[%u %u]",
 (unsigned int)min_qp, (unsigned int)max_qp, (unsigned int)session_qp_range.minqp, (unsigned int)session_qp_range.maxqp);
 }

 return true;
}
