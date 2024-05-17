OMX_ERRORTYPE omx_vdec::get_buffer_req(vdec_allocatorproperty *buffer_prop)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 struct v4l2_requestbuffers bufreq;
 unsigned int buf_size = 0, extra_data_size = 0, default_extra_data_size = 0;
 unsigned int final_extra_data_size = 0;
 struct v4l2_format fmt;
 int ret = 0;
    DEBUG_PRINT_LOW("GetBufReq IN: ActCnt(%d) Size(%u)",
            buffer_prop->actualcount, (unsigned int)buffer_prop->buffer_size);
    bufreq.memory = V4L2_MEMORY_USERPTR;
    bufreq.count = 1;
 if (buffer_prop->buffer_type == VDEC_BUFFER_TYPE_INPUT) {
        bufreq.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        fmt.type =V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        fmt.fmt.pix_mp.pixelformat = output_capability;
 } else if (buffer_prop->buffer_type == VDEC_BUFFER_TYPE_OUTPUT) {
        bufreq.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        fmt.type =V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        fmt.fmt.pix_mp.pixelformat = capture_capability;
 } else {
        eRet = OMX_ErrorBadParameter;
 }
 if (eRet==OMX_ErrorNone) {
        ret = ioctl(drv_ctx.video_driver_fd,VIDIOC_REQBUFS, &bufreq);
 }
 if (ret) {
        DEBUG_PRINT_ERROR("Requesting buffer requirements failed");
  
        eRet = OMX_ErrorInsufficientResources;
 return eRet;
 } else {
        buffer_prop->actualcount = bufreq.count;
        buffer_prop->mincount = bufreq.count;
        DEBUG_PRINT_HIGH("Count = %d",bufreq.count);
 }
    DEBUG_PRINT_LOW("GetBufReq IN: ActCnt(%d) Size(%u)",
            buffer_prop->actualcount, (unsigned int)buffer_prop->buffer_size);

    fmt.fmt.pix_mp.height = drv_ctx.video_resolution.frame_height;
    fmt.fmt.pix_mp.width = drv_ctx.video_resolution.frame_width;

    ret = ioctl(drv_ctx.video_driver_fd, VIDIOC_G_FMT, &fmt);

    update_resolution(fmt.fmt.pix_mp.width,
            fmt.fmt.pix_mp.height,
            fmt.fmt.pix_mp.plane_fmt[0].bytesperline,
            fmt.fmt.pix_mp.plane_fmt[0].reserved[0]);
 if (fmt.type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
        drv_ctx.num_planes = fmt.fmt.pix_mp.num_planes;
    DEBUG_PRINT_HIGH("Buffer Size = %d",fmt.fmt.pix_mp.plane_fmt[0].sizeimage);

 if (ret) {
  
        DEBUG_PRINT_ERROR("Requesting buffer requirements failed");
        eRet = OMX_ErrorInsufficientResources;
 } else {
 int extra_idx = 0;

        eRet = is_video_session_supported();
 if (eRet)
 return eRet;

        buffer_prop->buffer_size = fmt.fmt.pix_mp.plane_fmt[0].sizeimage;
        buf_size = buffer_prop->buffer_size;
        extra_idx = EXTRADATA_IDX(drv_ctx.num_planes);
 if (extra_idx && (extra_idx < VIDEO_MAX_PLANES)) {
            extra_data_size =  fmt.fmt.pix_mp.plane_fmt[extra_idx].sizeimage;
 } else if (extra_idx >= VIDEO_MAX_PLANES) {
            DEBUG_PRINT_ERROR("Extradata index is more than allowed: %d", extra_idx);
 return OMX_ErrorBadParameter;
 }

        default_extra_data_size = VENUS_EXTRADATA_SIZE(
                drv_ctx.video_resolution.frame_height,
                drv_ctx.video_resolution.frame_width);
        final_extra_data_size = extra_data_size > default_extra_data_size ?
            extra_data_size : default_extra_data_size;

        final_extra_data_size = (final_extra_data_size + buffer_prop->alignment - 1) &
 (~(buffer_prop->alignment - 1));

        drv_ctx.extradata_info.size = buffer_prop->actualcount * final_extra_data_size;
        drv_ctx.extradata_info.count = buffer_prop->actualcount;
        drv_ctx.extradata_info.buffer_size = final_extra_data_size;
 if (!secure_mode)
            buf_size += final_extra_data_size;
        buf_size = (buf_size + buffer_prop->alignment - 1)&(~(buffer_prop->alignment - 1));
        DEBUG_PRINT_LOW("GetBufReq UPDATE: ActCnt(%d) Size(%u) BufSize(%d)",
                buffer_prop->actualcount, (unsigned int)buffer_prop->buffer_size, buf_size);
 if (extra_data_size)
            DEBUG_PRINT_LOW("GetBufReq UPDATE: extradata: TotalSize(%d) BufferSize(%lu)",
                drv_ctx.extradata_info.size, drv_ctx.extradata_info.buffer_size);

 if (in_reconfig)  
            buffer_prop->buffer_size = buf_size;
 else if (buf_size != buffer_prop->buffer_size) {
            buffer_prop->buffer_size = buf_size;
            eRet = set_buffer_req(buffer_prop);
 }
 }
    DEBUG_PRINT_LOW("GetBufReq OUT: ActCnt(%d) Size(%u)",
            buffer_prop->actualcount, (unsigned int)buffer_prop->buffer_size);
 return eRet;
}
