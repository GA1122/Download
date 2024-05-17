int omx_vdec::stream_off(OMX_U32 port)
{
 enum v4l2_buf_type btype;
 int rc = 0;
 enum v4l2_ports v4l2_port = OUTPUT_PORT;

 if (port == OMX_CORE_INPUT_PORT_INDEX) {
        btype = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        v4l2_port = OUTPUT_PORT;
 } else if (port == OMX_CORE_OUTPUT_PORT_INDEX) {
        btype = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        v4l2_port = CAPTURE_PORT;
 } else if (port == OMX_ALL) {
 int rc_input = stream_off(OMX_CORE_INPUT_PORT_INDEX);
 int rc_output = stream_off(OMX_CORE_OUTPUT_PORT_INDEX);

 if (!rc_input)
 return rc_input;
 else
 return rc_output;
 }

 if (!streaming[v4l2_port]) {
        DEBUG_PRINT_HIGH("Warning: Attempting to stream off on %d port,"
 " which is already streamed off", v4l2_port);
 return 0;
 }

    DEBUG_PRINT_HIGH("Streaming off %d port", v4l2_port);

    rc = ioctl(drv_ctx.video_driver_fd, VIDIOC_STREAMOFF, &btype);
 if (rc) {
  
        DEBUG_PRINT_ERROR("Failed to call streamoff on %d Port", v4l2_port);
 } else {
        streaming[v4l2_port] = false;
 }

 return rc;
}
