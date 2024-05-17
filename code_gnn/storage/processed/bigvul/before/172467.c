bool venc_dev::venc_set_intra_vop_refresh(OMX_BOOL intra_vop_refresh)
{
    DEBUG_PRINT_LOW("venc_set_intra_vop_refresh: intra_vop = %uc", intra_vop_refresh);

 if (intra_vop_refresh == OMX_TRUE) {
 struct v4l2_control control;
 int rc;
        control.id = V4L2_CID_MPEG_VIDC_VIDEO_REQUEST_IFRAME;
        control.value = 1;
       DEBUG_PRINT_ERROR("Calling IOCTL set control for id=%x, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
           DEBUG_PRINT_ERROR("Failed to set Intra Frame Request control");
 return false;
 }

       DEBUG_PRINT_ERROR("Success IOCTL set control for id=%x, value=%d", control.id, control.value);
 } else {
        DEBUG_PRINT_ERROR("ERROR: VOP Refresh is False, no effect");
 }

 return true;
}
