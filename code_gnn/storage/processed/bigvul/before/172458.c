bool venc_dev::venc_set_extradata(OMX_U32 extra_data, OMX_BOOL enable)
{
 struct v4l2_control control;

    DEBUG_PRINT_HIGH("venc_set_extradata:: %x", (int) extra_data);

 if (enable == OMX_FALSE) {
  
 return false;
 }

    control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
 switch (extra_data) {
 case OMX_ExtraDataVideoEncoderSliceInfo:
            control.value = V4L2_MPEG_VIDC_EXTRADATA_MULTISLICE_INFO;
 break;
 case OMX_ExtraDataVideoEncoderMBInfo:
            control.value = V4L2_MPEG_VIDC_EXTRADATA_METADATA_MBI;
 break;
 default:
            DEBUG_PRINT_ERROR("Unrecognized extradata index 0x%x", (unsigned int)extra_data);
 return false;
 }

 if (ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control)) {
        DEBUG_PRINT_ERROR("ERROR: Request for setting extradata (%x) failed %d",
 (unsigned int)extra_data, errno);
 return false;
 }

 return true;
}
