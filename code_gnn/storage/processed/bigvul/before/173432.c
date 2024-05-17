OMX_ERRORTYPE omx_vdec::enable_extradata(OMX_U32 requested_extradata,
 bool is_internal, bool enable)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
 struct v4l2_control control;
 if (m_state != OMX_StateLoaded) {
        DEBUG_PRINT_ERROR("ERROR: enable extradata allowed in Loaded state only");
 return OMX_ErrorIncorrectStateOperation;
 }

    DEBUG_PRINT_HIGH("NOTE: enable_extradata: actual[%u] requested[%u] enable[%d], is_internal: %d",
 (unsigned int)client_extradata, (unsigned int)requested_extradata, enable, is_internal);

 if (!is_internal) {
 if (enable)
            client_extradata |= requested_extradata;
 else
            client_extradata = client_extradata & ~requested_extradata;
 }

 if (enable) {
 if (requested_extradata & OMX_INTERLACE_EXTRADATA) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_INTERLACE_VIDEO;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set interlaced extradata."
 " Quality of interlaced clips might be impacted.");
 }
 }
 if (requested_extradata & OMX_FRAMEINFO_EXTRADATA) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_FRAME_RATE;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set framerate extradata");
 }
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_NUM_CONCEALED_MB;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set concealed MB extradata");
 }
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_RECOVERY_POINT_SEI;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set recovery point SEI extradata");
 }
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_PANSCAN_WINDOW;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set panscan extradata");
 }
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_ASPECT_RATIO;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set panscan extradata");
 }
 }
 if (requested_extradata & OMX_TIMEINFO_EXTRADATA) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_TIMESTAMP;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set timeinfo extradata");
 }
 }
 if (requested_extradata & OMX_FRAMEPACK_EXTRADATA) {
 if (output_capability == V4L2_PIX_FMT_H264) {
                DEBUG_PRINT_HIGH("enable OMX_FRAMEPACK_EXTRADATA");
                control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
                control.value =  V4L2_MPEG_VIDC_EXTRADATA_S3D_FRAME_PACKING;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                    DEBUG_PRINT_HIGH("Failed to set S3D_FRAME_PACKING extradata");
 }
 } else {
                DEBUG_PRINT_HIGH("OMX_FRAMEPACK_EXTRADATA supported for H264 only");
 }
 }
 if (requested_extradata & OMX_QP_EXTRADATA) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_FRAME_QP;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set QP extradata");
 }
 }
 if (requested_extradata & OMX_BITSINFO_EXTRADATA) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_FRAME_BITS_INFO;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set frame bits info extradata");
 }
 }
 if (requested_extradata & OMX_EXTNUSER_EXTRADATA) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_STREAM_USERDATA;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set stream userdata extradata");
 }
 }
 if (requested_extradata & OMX_MPEG2SEQDISP_EXTRADATA) {
 if (output_capability == V4L2_PIX_FMT_MPEG2) {
                DEBUG_PRINT_HIGH("Enable seq display extradata");
                control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
                control.value =  V4L2_MPEG_VIDC_EXTRADATA_MPEG2_SEQDISP;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                    DEBUG_PRINT_HIGH("Failed to set seqdisp extradata");
 }
 } else {
                DEBUG_PRINT_HIGH("Seq display extradata is supported for MPEG2 only");
 }
 }
 if (requested_extradata & OMX_VUI_DISPLAY_INFO_EXTRADATA) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA;
            control.value = V4L2_MPEG_VIDC_EXTRADATA_VUI_DISPLAY;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_S_CTRL, &control)) {
                DEBUG_PRINT_HIGH("Failed to set display VUI extradata");
 }
 }
 }
    ret = get_buffer_req(&drv_ctx.op_buf);
 return ret;
}
