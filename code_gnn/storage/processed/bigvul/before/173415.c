int omx_vdec::async_message_process (void *context, void* message)
{
    omx_vdec* omx = NULL;
 struct vdec_msginfo *vdec_msg = NULL;
    OMX_BUFFERHEADERTYPE* omxhdr = NULL;
 struct v4l2_buffer *v4l2_buf_ptr = NULL;
 struct vdec_output_frameinfo *output_respbuf = NULL;
 int rc=1;
 if (context == NULL || message == NULL) {
        DEBUG_PRINT_ERROR("FATAL ERROR in omx_vdec::async_message_process NULL Check");
 return -1;
 }
    vdec_msg = (struct vdec_msginfo *)message;

    omx = reinterpret_cast<omx_vdec*>(context);

 switch (vdec_msg->msgcode) {

 case VDEC_MSG_EVT_HW_ERROR:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_HARDWARE_ERROR);
 break;

 case VDEC_MSG_EVT_HW_OVERLOAD:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_HARDWARE_OVERLOAD);
 break;

 case VDEC_MSG_EVT_HW_UNSUPPORTED:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_UNSUPPORTED_SETTING);
 break;

 case VDEC_MSG_RESP_START_DONE:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_START_DONE);
 break;

 case VDEC_MSG_RESP_STOP_DONE:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_STOP_DONE);
 break;

 case VDEC_MSG_RESP_RESUME_DONE:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_RESUME_DONE);
 break;

 case VDEC_MSG_RESP_PAUSE_DONE:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_PAUSE_DONE);
 break;

 case VDEC_MSG_RESP_FLUSH_INPUT_DONE:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_EVENT_INPUT_FLUSH);
 break;
 case VDEC_MSG_RESP_FLUSH_OUTPUT_DONE:
            omx->post_event ((unsigned)NULL, vdec_msg->status_code,\
                    OMX_COMPONENT_GENERATE_EVENT_OUTPUT_FLUSH);
 break;
 case VDEC_MSG_RESP_INPUT_FLUSHED:
 case VDEC_MSG_RESP_INPUT_BUFFER_DONE:

  

            v4l2_buf_ptr = (v4l2_buffer*)vdec_msg->msgdata.input_frame_clientdata;
            omxhdr=omx->m_inp_mem_ptr+v4l2_buf_ptr->index;
 if (omxhdr == NULL ||
 ((omxhdr - omx->m_inp_mem_ptr) > (int)omx->drv_ctx.ip_buf.actualcount) ) {
                omxhdr = NULL;
                vdec_msg->status_code = VDEC_S_EFATAL;
 break;
 }
 if (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_INPUT_UNSUPPORTED) {
                DEBUG_PRINT_HIGH("Unsupported input");
                omx->omx_report_error ();
 }
 if (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_DATA_CORRUPT) {
                omxhdr->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                vdec_msg->status_code = VDEC_S_INPUT_BITSTREAM_ERR;
 }
 if (omxhdr->nFlags & OMX_BUFFERFLAG_CODECCONFIG) {

                DEBUG_PRINT_LOW("Decrement codec_config buffer counter");
                android_atomic_dec(&omx->m_queued_codec_config_count);
 if ((android_atomic_add(0, &omx->m_queued_codec_config_count) == 0) &&
                    BITMASK_PRESENT(&omx->m_flags, OMX_COMPONENT_FLUSH_DEFERRED)) {
                    DEBUG_PRINT_LOW("sem post for CODEC CONFIG buffer");
                    sem_post(&omx->m_safe_flush);
 }
 }

            omx->post_event ((unsigned long)omxhdr,vdec_msg->status_code,
                    OMX_COMPONENT_GENERATE_EBD);
 break;
 case VDEC_MSG_EVT_INFO_FIELD_DROPPED:
 int64_t *timestamp;
            timestamp = (int64_t *) malloc(sizeof(int64_t));
 if (timestamp) {
 *timestamp = vdec_msg->msgdata.output_frame.time_stamp;
                omx->post_event ((unsigned long)timestamp, vdec_msg->status_code,
                        OMX_COMPONENT_GENERATE_INFO_FIELD_DROPPED);
                DEBUG_PRINT_HIGH("Field dropped time stamp is %lld",
 (long long)vdec_msg->msgdata.output_frame.time_stamp);
 }
 break;
 case VDEC_MSG_RESP_OUTPUT_FLUSHED:
 case VDEC_MSG_RESP_OUTPUT_BUFFER_DONE:

            v4l2_buf_ptr = (v4l2_buffer*)vdec_msg->msgdata.output_frame.client_data;
            omxhdr=omx->m_out_mem_ptr+v4l2_buf_ptr->index;

            DEBUG_PRINT_LOW("[RespBufDone] Buf(%p) Ts(%lld) PicType(%u) Flags (0x%x) FillLen(%u) Crop: L(%u) T(%u) R(%u) B(%u)",
                    omxhdr, (long long)vdec_msg->msgdata.output_frame.time_stamp,
                    vdec_msg->msgdata.output_frame.pic_type, v4l2_buf_ptr->flags,
 (unsigned int)vdec_msg->msgdata.output_frame.len,
                    vdec_msg->msgdata.output_frame.framesize.left,
                    vdec_msg->msgdata.output_frame.framesize.top,
                    vdec_msg->msgdata.output_frame.framesize.right,
                    vdec_msg->msgdata.output_frame.framesize.bottom);

 if (omxhdr && omxhdr->pOutputPortPrivate &&
 ((omxhdr - omx->m_out_mem_ptr) < (int)omx->drv_ctx.op_buf.actualcount) &&
 (((struct vdec_output_frameinfo *)omxhdr->pOutputPortPrivate
 - omx->drv_ctx.ptr_respbuffer) < (int)omx->drv_ctx.op_buf.actualcount)) {

 if ( vdec_msg->msgdata.output_frame.len <=  omxhdr->nAllocLen) {
                    omxhdr->nFilledLen = vdec_msg->msgdata.output_frame.len;
                    omxhdr->nOffset = vdec_msg->msgdata.output_frame.offset;
                    omxhdr->nTimeStamp = vdec_msg->msgdata.output_frame.time_stamp;
                    omxhdr->nFlags = 0;

 if (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_EOS) {
                        omxhdr->nFlags |= OMX_BUFFERFLAG_EOS;
 }
 if (omxhdr->nFilledLen) {
                        omxhdr->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
 }
 if (v4l2_buf_ptr->flags & V4L2_BUF_FLAG_KEYFRAME || v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_IDRFRAME) {
                        omxhdr->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
 } else {
                        omxhdr->nFlags &= ~OMX_BUFFERFLAG_SYNCFRAME;
 }
 if (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_EOSEQ) {
                        omxhdr->nFlags |= QOMX_VIDEO_BUFFERFLAG_EOSEQ;
 }
 if (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_DECODEONLY) {
                        omxhdr->nFlags |= OMX_BUFFERFLAG_DECODEONLY;
 }

 if (v4l2_buf_ptr->flags & V4L2_MSM_BUF_FLAG_MBAFF) {
                        omxhdr->nFlags |= QOMX_VIDEO_BUFFERFLAG_MBAFF;
 }

 if (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_READONLY) {
                         omxhdr->nFlags |= OMX_BUFFERFLAG_READONLY;
                         DEBUG_PRINT_LOW("F_B_D: READONLY BUFFER - REFERENCE WITH F/W fd = %d",
                                    omx->drv_ctx.ptr_outputbuffer[v4l2_buf_ptr->index].pmem_fd);
 }

 if (omx->dynamic_buf_mode && !(v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_READONLY)) {
                        omx->buf_ref_remove(omx->drv_ctx.ptr_outputbuffer[v4l2_buf_ptr->index].pmem_fd,
                            omxhdr->nOffset);
 }
 if (omxhdr && (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_DROP_FRAME) &&
 !(v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_DECODEONLY) &&
 !(v4l2_buf_ptr->flags & V4L2_QCOM_BUF_FLAG_EOS)) {
                        omx->time_stamp_dts.remove_time_stamp(
                                omxhdr->nTimeStamp,
 (omx->drv_ctx.interlace != VDEC_InterlaceFrameProgressive)
 ?true:false);
                        omx->post_event ((unsigned long)NULL,(unsigned long)omxhdr,
                                OMX_COMPONENT_GENERATE_FTB);
 break;
 }
 if (v4l2_buf_ptr->flags & V4L2_QCOM_BUF_DATA_CORRUPT) {
                        omxhdr->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
 }
                    vdec_msg->msgdata.output_frame.bufferaddr =
                        omx->drv_ctx.ptr_outputbuffer[v4l2_buf_ptr->index].bufferaddr;

  
  
  
 if (omxhdr->nFilledLen
 && ((omx->prev_n_filled_len != omxhdr->nFilledLen)
 || (omx->drv_ctx.frame_size.left != vdec_msg->msgdata.output_frame.framesize.left)
 || (omx->drv_ctx.frame_size.top != vdec_msg->msgdata.output_frame.framesize.top)
 || (omx->drv_ctx.frame_size.right != vdec_msg->msgdata.output_frame.framesize.right)
 || (omx->drv_ctx.frame_size.bottom != vdec_msg->msgdata.output_frame.framesize.bottom)
 || (omx->drv_ctx.video_resolution.frame_width != vdec_msg->msgdata.output_frame.picsize.frame_width)
 || (omx->drv_ctx.video_resolution.frame_height != vdec_msg->msgdata.output_frame.picsize.frame_height) )) {

                        DEBUG_PRINT_HIGH("Paramters Changed From: Len: %u, WxH: %dx%d, L: %u, T: %u, R: %u, B: %u --> Len: %u, WxH: %dx%d, L: %u, T: %u, R: %u, B: %u",
                                omx->prev_n_filled_len,
                                omx->drv_ctx.video_resolution.frame_width,
                                omx->drv_ctx.video_resolution.frame_height,
                                omx->drv_ctx.frame_size.left, omx->drv_ctx.frame_size.top,
                                omx->drv_ctx.frame_size.right, omx->drv_ctx.frame_size.bottom,
                                omxhdr->nFilledLen, vdec_msg->msgdata.output_frame.picsize.frame_width,
                                vdec_msg->msgdata.output_frame.picsize.frame_height,
                                        vdec_msg->msgdata.output_frame.framesize.left,
                                        vdec_msg->msgdata.output_frame.framesize.top,
                                        vdec_msg->msgdata.output_frame.framesize.right,
                                vdec_msg->msgdata.output_frame.framesize.bottom);

                        omx->drv_ctx.video_resolution.frame_width =
                                vdec_msg->msgdata.output_frame.picsize.frame_width;
                        omx->drv_ctx.video_resolution.frame_height =
                                vdec_msg->msgdata.output_frame.picsize.frame_height;
 if (omx->drv_ctx.output_format == VDEC_YUV_FORMAT_NV12) {
                            omx->drv_ctx.video_resolution.stride =
                                VENUS_Y_STRIDE(COLOR_FMT_NV12, omx->drv_ctx.video_resolution.frame_width);
                            omx->drv_ctx.video_resolution.scan_lines =
                                VENUS_Y_SCANLINES(COLOR_FMT_NV12, omx->drv_ctx.video_resolution.frame_height);
 }
                        memcpy(&omx->drv_ctx.frame_size,
 &vdec_msg->msgdata.output_frame.framesize,
 sizeof(struct vdec_framesize));

                        omx->post_event(OMX_CORE_OUTPUT_PORT_INDEX,
                                OMX_IndexConfigCommonOutputCrop,
                                    OMX_COMPONENT_GENERATE_PORT_RECONFIG);
 }

 if (omxhdr->nFilledLen)
                        omx->prev_n_filled_len = omxhdr->nFilledLen;

                    output_respbuf = (struct vdec_output_frameinfo *)\
                             omxhdr->pOutputPortPrivate;
                    output_respbuf->len = vdec_msg->msgdata.output_frame.len;
                    output_respbuf->offset = vdec_msg->msgdata.output_frame.offset;
 if (v4l2_buf_ptr->flags & V4L2_BUF_FLAG_KEYFRAME) {
                        output_respbuf->pic_type = PICTURE_TYPE_I;
 }
 if (v4l2_buf_ptr->flags & V4L2_BUF_FLAG_PFRAME) {
                        output_respbuf->pic_type = PICTURE_TYPE_P;
 }
 if (v4l2_buf_ptr->flags & V4L2_BUF_FLAG_BFRAME) {
                        output_respbuf->pic_type = PICTURE_TYPE_B;
 }

 if (omx->output_use_buffer)
                        memcpy ( omxhdr->pBuffer, (void *)
 ((unsigned long)vdec_msg->msgdata.output_frame.bufferaddr +
 (unsigned long)vdec_msg->msgdata.output_frame.offset),
                                vdec_msg->msgdata.output_frame.len);
 } else {
                    DEBUG_PRINT_ERROR("Invalid filled length = %u, buffer size = %u, prev_length = %u",
 (unsigned int)vdec_msg->msgdata.output_frame.len,
                            omxhdr->nAllocLen, omx->prev_n_filled_len);
                    omxhdr->nFilledLen = 0;
 }

                omx->post_event ((unsigned long)omxhdr, vdec_msg->status_code,
                        OMX_COMPONENT_GENERATE_FBD);

 } else if (vdec_msg->msgdata.output_frame.flags & OMX_BUFFERFLAG_EOS) {
                omx->post_event ((unsigned long)NULL, vdec_msg->status_code,
                        OMX_COMPONENT_GENERATE_EOS_DONE);
 } else {
                omx->post_event ((unsigned int)NULL, vdec_msg->status_code,
                        OMX_COMPONENT_GENERATE_HARDWARE_ERROR);
 }
 break;
 case VDEC_MSG_EVT_CONFIG_CHANGED:
            DEBUG_PRINT_HIGH("Port settings changed");
            omx->post_event (OMX_CORE_OUTPUT_PORT_INDEX, OMX_IndexParamPortDefinition,
                    OMX_COMPONENT_GENERATE_PORT_RECONFIG);
 break;
 default:
 break;
 }
 return rc;
}
