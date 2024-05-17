bool omx_vdec::execute_omx_flush(OMX_U32 flushType)
{
 bool bRet = false;
 struct v4l2_plane plane;
 struct v4l2_buffer v4l2_buf;
 struct v4l2_decoder_cmd dec;
    DEBUG_PRINT_LOW("in %s, flushing %u", __func__, (unsigned int)flushType);
    memset((void *)&v4l2_buf,0,sizeof(v4l2_buf));
    dec.cmd = V4L2_DEC_QCOM_CMD_FLUSH;

    DEBUG_PRINT_HIGH("in %s: reconfig? %d", __func__, in_reconfig);

 if (in_reconfig && flushType == OMX_CORE_OUTPUT_PORT_INDEX) {
        output_flush_progress = true;
        dec.flags = V4L2_DEC_QCOM_CMD_FLUSH_CAPTURE;
 } else {
  
        input_flush_progress = true;
        output_flush_progress = true;
        dec.flags = V4L2_DEC_QCOM_CMD_FLUSH_OUTPUT | V4L2_DEC_QCOM_CMD_FLUSH_CAPTURE;
 }

 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_DECODER_CMD, &dec)) {
        DEBUG_PRINT_ERROR("Flush Port (%u) Failed ", (unsigned int)flushType);
        bRet = false;
 }

 return bRet;
}
