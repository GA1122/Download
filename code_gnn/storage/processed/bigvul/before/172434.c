bool venc_dev::venc_get_buf_req(OMX_U32 *min_buff_count,
        OMX_U32 *actual_buff_count,
        OMX_U32 *buff_size,
        OMX_U32 port)
{
 struct v4l2_format fmt;
 struct v4l2_requestbuffers bufreq;
 unsigned int buf_size = 0, extra_data_size = 0, client_extra_data_size = 0;
 int ret;

 if (port == 0) {
        fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        fmt.fmt.pix_mp.height = m_sVenc_cfg.input_height;
        fmt.fmt.pix_mp.width = m_sVenc_cfg.input_width;
        fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12;
        fmt.fmt.pix_mp.colorspace = V4L2_COLORSPACE_BT878;
        ret = ioctl(m_nDriver_fd, VIDIOC_G_FMT, &fmt);
        m_sInput_buff_property.datasize=fmt.fmt.pix_mp.plane_fmt[0].sizeimage;
        bufreq.memory = V4L2_MEMORY_USERPTR;

 if (*actual_buff_count)
            bufreq.count = *actual_buff_count;
 else
            bufreq.count = 2;

 if (metadatamode && (bufreq.count < 9)) {
            DEBUG_PRINT_LOW("FW returned buffer count = %d , overwriting with 9",
                bufreq.count);
            bufreq.count = 9;
 }
 if (m_sVenc_cfg.input_height * m_sVenc_cfg.input_width >= 3840*2160) {
            DEBUG_PRINT_LOW("Increasing buffer count = %d to 11", bufreq.count);
            bufreq.count = 11;
 } else {
            bufreq.count = 12;
 }

        bufreq.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        ret = ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq);

 if (ret) {
            DEBUG_PRINT_ERROR("VIDIOC_REQBUFS OUTPUT_MPLANE Failed");
 return false;
 }

        m_sInput_buff_property.mincount = m_sInput_buff_property.actualcount = bufreq.count;
 *min_buff_count = m_sInput_buff_property.mincount;
 *actual_buff_count = m_sInput_buff_property.actualcount;
#ifdef USE_ION
        m_sInput_buff_property.datasize = ALIGN(m_sInput_buff_property.datasize, SZ_4K);
#endif
 *buff_size = m_sInput_buff_property.datasize;
 } else {
 unsigned int extra_idx = 0;
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        fmt.fmt.pix_mp.height = m_sVenc_cfg.dvs_height;
        fmt.fmt.pix_mp.width = m_sVenc_cfg.dvs_width;
        fmt.fmt.pix_mp.pixelformat = m_sVenc_cfg.codectype;

        ret = ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt);
        m_sOutput_buff_property.datasize=fmt.fmt.pix_mp.plane_fmt[0].sizeimage;
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        fmt.fmt.pix_mp.height = m_sVenc_cfg.dvs_height;
        fmt.fmt.pix_mp.width = m_sVenc_cfg.dvs_width;
        fmt.fmt.pix_mp.pixelformat = m_sVenc_cfg.codectype;

        ret = ioctl(m_nDriver_fd, VIDIOC_G_FMT, &fmt);
        m_sOutput_buff_property.datasize=fmt.fmt.pix_mp.plane_fmt[0].sizeimage;
        bufreq.memory = V4L2_MEMORY_USERPTR;

 if (*actual_buff_count)
            bufreq.count = *actual_buff_count;
 else
            bufreq.count = 2;

        bufreq.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        ret = ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq);

 if (ret) {
            DEBUG_PRINT_ERROR("VIDIOC_REQBUFS CAPTURE_MPLANE Failed");
 return false;
 }

        m_sOutput_buff_property.mincount = m_sOutput_buff_property.actualcount = bufreq.count;
 *min_buff_count = m_sOutput_buff_property.mincount;
 *actual_buff_count = m_sOutput_buff_property.actualcount;
 *buff_size = m_sOutput_buff_property.datasize;
        num_planes = fmt.fmt.pix_mp.num_planes;
        extra_idx = EXTRADATA_IDX(num_planes);

 if (extra_idx && (extra_idx < VIDEO_MAX_PLANES)) {
            extra_data_size =  fmt.fmt.pix_mp.plane_fmt[extra_idx].sizeimage;
 } else if (extra_idx >= VIDEO_MAX_PLANES) {
            DEBUG_PRINT_ERROR("Extradata index is more than allowed: %d", extra_idx);
 return OMX_ErrorBadParameter;
 }

        extradata_info.buffer_size = extra_data_size;
        extradata_info.count = m_sOutput_buff_property.actualcount;
        extradata_info.size = extradata_info.buffer_size * extradata_info.count;
 }

 return true;
}
