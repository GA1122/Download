void omx_vdec::handle_extradata(OMX_BUFFERHEADERTYPE *p_buf_hdr)
{
    OMX_OTHER_EXTRADATATYPE *p_extra = NULL, *p_sei = NULL, *p_vui = NULL;
    OMX_U32 num_conceal_MB = 0;
    OMX_TICKS time_stamp = 0;
    OMX_U32 frame_rate = 0;
 unsigned long consumed_len = 0;
    OMX_U32 num_MB_in_frame;
    OMX_U32 recovery_sei_flags = 1;
 int enable = 0;

 int buf_index = p_buf_hdr - m_out_mem_ptr;
 if (buf_index >= drv_ctx.extradata_info.count) {
        DEBUG_PRINT_ERROR("handle_extradata: invalid index(%d) max(%d)",
                buf_index, drv_ctx.extradata_info.count);
 return;
 }
 struct msm_vidc_panscan_window_payload *panscan_payload = NULL;
    OMX_U8 *pBuffer = (OMX_U8 *)(drv_ctx.ptr_outputbuffer[buf_index].bufferaddr) +
        p_buf_hdr->nOffset;

 if (!drv_ctx.extradata_info.uaddr) {
        DEBUG_PRINT_HIGH("NULL drv_ctx.extradata_info.uaddr");
 return;
 }
 if (!secure_mode && (drv_ctx.extradata_info.buffer_size > (p_buf_hdr->nAllocLen - p_buf_hdr->nFilledLen)) ) {
        DEBUG_PRINT_ERROR("Error: Insufficient size allocated for extra-data");
        p_extra = NULL;
 return;
 }
 if (!secure_mode)
        p_extra = (OMX_OTHER_EXTRADATATYPE *)
 ((unsigned long)(pBuffer + p_buf_hdr->nOffset + p_buf_hdr->nFilledLen + 3)&(~3));
 else
        p_extra = m_other_extradata;
 char *p_extradata = drv_ctx.extradata_info.uaddr + buf_index * drv_ctx.extradata_info.buffer_size;

 if (!secure_mode && ((OMX_U8*)p_extra > (pBuffer + p_buf_hdr->nAllocLen))) {
        p_extra = NULL;
        DEBUG_PRINT_ERROR("Error: out of bound memory access by p_extra");
 return;
 }
    OMX_OTHER_EXTRADATATYPE *data = (struct OMX_OTHER_EXTRADATATYPE *)p_extradata;
 if (data && p_extra) {
 while ((consumed_len < drv_ctx.extradata_info.buffer_size)
 && (data->eType != (OMX_EXTRADATATYPE)MSM_VIDC_EXTRADATA_NONE)) {
 if ((consumed_len + data->nSize) > (unsigned)drv_ctx.extradata_info.buffer_size) {
                DEBUG_PRINT_LOW("Invalid extra data size");
 break;
 }
            DEBUG_PRINT_LOW("handle_extradata: eType = %d", data->eType);
 switch ((unsigned long)data->eType) {
 case MSM_VIDC_EXTRADATA_INTERLACE_VIDEO:
 struct msm_vidc_interlace_payload *payload;
                    payload = (struct msm_vidc_interlace_payload *)(void *)data->data;
 if (payload) {
                        enable = 1;
 switch (payload->format) {
 case MSM_VIDC_INTERLACE_FRAME_PROGRESSIVE:
                                drv_ctx.interlace = VDEC_InterlaceFrameProgressive;
                                enable = 0;
 break;
 case MSM_VIDC_INTERLACE_INTERLEAVE_FRAME_TOPFIELDFIRST:
                                drv_ctx.interlace = VDEC_InterlaceInterleaveFrameTopFieldFirst;
 break;
 case MSM_VIDC_INTERLACE_INTERLEAVE_FRAME_BOTTOMFIELDFIRST:
                                drv_ctx.interlace = VDEC_InterlaceInterleaveFrameBottomFieldFirst;
 break;
 default:
                                DEBUG_PRINT_LOW("default case - set interlace to topfield");
                                drv_ctx.interlace = VDEC_InterlaceInterleaveFrameTopFieldFirst;
 }
 }

 if (m_enable_android_native_buffers) {
                        DEBUG_PRINT_LOW("setMetaData INTERLACED format:%d enable:%d mbaff:%d",
                                         payload->format, enable,
 (p_buf_hdr->nFlags & QOMX_VIDEO_BUFFERFLAG_MBAFF)?true:false);
                        setMetaData((private_handle_t *)native_buffer[buf_index].privatehandle,
                               PP_PARAM_INTERLACED, (void*)&enable);
 }
 if (client_extradata & OMX_INTERLACE_EXTRADATA) {
                        append_interlace_extradata(p_extra, payload->format,
                                      p_buf_hdr->nFlags & QOMX_VIDEO_BUFFERFLAG_MBAFF);
                        p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 break;
 case MSM_VIDC_EXTRADATA_FRAME_RATE:
 struct msm_vidc_framerate_payload *frame_rate_payload;
                    frame_rate_payload = (struct msm_vidc_framerate_payload *)(void *)data->data;
                    frame_rate = frame_rate_payload->frame_rate;
 break;
 case MSM_VIDC_EXTRADATA_TIMESTAMP:
 struct msm_vidc_ts_payload *time_stamp_payload;
                    time_stamp_payload = (struct msm_vidc_ts_payload *)(void *)data->data;
                    time_stamp = time_stamp_payload->timestamp_lo;
                    time_stamp |= ((unsigned long long)time_stamp_payload->timestamp_hi << 32);
                    p_buf_hdr->nTimeStamp = time_stamp;
 break;
 case MSM_VIDC_EXTRADATA_NUM_CONCEALED_MB:
 struct msm_vidc_concealmb_payload *conceal_mb_payload;
                    conceal_mb_payload = (struct msm_vidc_concealmb_payload *)(void *)data->data;
                    num_MB_in_frame = ((drv_ctx.video_resolution.frame_width + 15) *
 (drv_ctx.video_resolution.frame_height + 15)) >> 8;
                    num_conceal_MB = ((num_MB_in_frame > 0)?(conceal_mb_payload->num_mbs * 100 / num_MB_in_frame) : 0);
 break;
 case MSM_VIDC_EXTRADATA_INDEX:
 int *etype;
                    etype  = (int *)(void *)data->data;
 if (etype && *etype == MSM_VIDC_EXTRADATA_ASPECT_RATIO) {
 struct msm_vidc_aspect_ratio_payload *aspect_ratio_payload;
                        aspect_ratio_payload = (struct msm_vidc_aspect_ratio_payload *)(++etype);
 if (aspect_ratio_payload) {
 ((struct vdec_output_frameinfo *)
                             p_buf_hdr->pOutputPortPrivate)->aspect_ratio_info.par_width = aspect_ratio_payload->aspect_width;
 ((struct vdec_output_frameinfo *)
                             p_buf_hdr->pOutputPortPrivate)->aspect_ratio_info.par_height = aspect_ratio_payload->aspect_height;
 }
 }
 break;
 case MSM_VIDC_EXTRADATA_RECOVERY_POINT_SEI:
 struct msm_vidc_recoverysei_payload *recovery_sei_payload;
                    recovery_sei_payload = (struct msm_vidc_recoverysei_payload *)(void *)data->data;
                    recovery_sei_flags = recovery_sei_payload->flags;
 if (recovery_sei_flags != MSM_VIDC_FRAME_RECONSTRUCTION_CORRECT) {
                        p_buf_hdr->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                        DEBUG_PRINT_HIGH("***************************************************");
                        DEBUG_PRINT_HIGH("FillBufferDone: OMX_BUFFERFLAG_DATACORRUPT Received");
                        DEBUG_PRINT_HIGH("***************************************************");
 }
 break;
 case MSM_VIDC_EXTRADATA_PANSCAN_WINDOW:
                    panscan_payload = (struct msm_vidc_panscan_window_payload *)(void *)data->data;
 if (panscan_payload->num_panscan_windows > MAX_PAN_SCAN_WINDOWS) {
                        DEBUG_PRINT_ERROR("Panscan windows are more than supported\n");
                        DEBUG_PRINT_ERROR("Max supported = %d FW returned = %d\n",
                            MAX_PAN_SCAN_WINDOWS, panscan_payload->num_panscan_windows);
 return;
 }
 break;
 case MSM_VIDC_EXTRADATA_MPEG2_SEQDISP:
 struct msm_vidc_mpeg2_seqdisp_payload *seqdisp_payload;
                    seqdisp_payload = (struct msm_vidc_mpeg2_seqdisp_payload *)(void *)data->data;
 if (seqdisp_payload) {
                        m_disp_hor_size = seqdisp_payload->disp_width;
                        m_disp_vert_size = seqdisp_payload->disp_height;
 if (client_extradata & OMX_MPEG2SEQDISP_EXTRADATA) {
                            append_mpeg2_seqdisplay_extradata(p_extra, seqdisp_payload);
                            p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 }
 break;
 case MSM_VIDC_EXTRADATA_S3D_FRAME_PACKING:
 struct msm_vidc_s3d_frame_packing_payload *s3d_frame_packing_payload;
                    s3d_frame_packing_payload = (struct msm_vidc_s3d_frame_packing_payload *)(void *)data->data;
 if (client_extradata & OMX_FRAMEPACK_EXTRADATA) {
                        append_framepack_extradata(p_extra, s3d_frame_packing_payload);
                        p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 break;
 case MSM_VIDC_EXTRADATA_FRAME_QP:
 struct msm_vidc_frame_qp_payload *qp_payload;
                    qp_payload = (struct msm_vidc_frame_qp_payload*)(void *)data->data;
 if (client_extradata & OMX_QP_EXTRADATA) {
                        append_qp_extradata(p_extra, qp_payload);
                        p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 break;
 case MSM_VIDC_EXTRADATA_FRAME_BITS_INFO:
 struct msm_vidc_frame_bits_info_payload *bits_info_payload;
                    bits_info_payload = (struct msm_vidc_frame_bits_info_payload*)(void *)data->data;
 if (client_extradata & OMX_BITSINFO_EXTRADATA) {
                        append_bitsinfo_extradata(p_extra, bits_info_payload);
                        p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 break;
 case MSM_VIDC_EXTRADATA_STREAM_USERDATA:
 if (client_extradata & OMX_EXTNUSER_EXTRADATA) {
                        append_user_extradata(p_extra, data);
                        p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 break;
 case MSM_VIDC_EXTRADATA_VUI_DISPLAY_INFO:
 struct msm_vidc_vui_display_info_payload *display_info_payload;
                    display_info_payload = (struct msm_vidc_vui_display_info_payload*)(void *)data->data;

 if (client_extradata & OMX_VUI_DISPLAY_INFO_EXTRADATA) {
  
                        DEBUG_PRINT_ERROR("VUI display info not propagated to client");
 }

 if (m_enable_android_native_buffers
 && display_info_payload->video_signal_present_flag
 && display_info_payload->color_description_present_flag) {
 ColorSpace_t color_space = ITU_R_601;

 switch (display_info_payload->color_primaries) {
 case 1:
                                color_space = ITU_R_709;
 break;
 case 5:
                                color_space = display_info_payload->video_full_range_flag ?
                                    ITU_R_601_FR : ITU_R_601;
 break;
 }

                        DEBUG_PRINT_LOW("colorspace from VUI = %d", color_space);
                        setMetaData((private_handle_t *)native_buffer[buf_index].privatehandle,
                               UPDATE_COLOR_SPACE, (void*)&color_space);
 }
 break;
 default:
                    DEBUG_PRINT_LOW("Unrecognized extradata");
 goto unrecognized_extradata;
 }
            consumed_len += data->nSize;
            data = (OMX_OTHER_EXTRADATATYPE *)((char *)data + data->nSize);
 }
 if (client_extradata & OMX_FRAMEINFO_EXTRADATA) {
            p_buf_hdr->nFlags |= OMX_BUFFERFLAG_EXTRADATA;
            append_frame_info_extradata(p_extra,
                    num_conceal_MB, ((struct vdec_output_frameinfo *)p_buf_hdr->pOutputPortPrivate)->pic_type, frame_rate,
                    time_stamp, panscan_payload,&((struct vdec_output_frameinfo *)
                        p_buf_hdr->pOutputPortPrivate)->aspect_ratio_info);
            p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 if (client_extradata & OMX_FRAMEDIMENSION_EXTRADATA) {
            append_frame_dimension_extradata(p_extra);
            p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 }
unrecognized_extradata:
 if (client_extradata && p_extra) {
        p_buf_hdr->nFlags |= OMX_BUFFERFLAG_EXTRADATA;
        append_terminator_extradata(p_extra);
 }
 if (secure_mode && p_extradata && m_other_extradata) {
 struct vdec_output_frameinfo  *ptr_extradatabuff = NULL;
        memcpy(p_extradata, m_other_extradata, drv_ctx.extradata_info.buffer_size);
        ptr_extradatabuff = (struct vdec_output_frameinfo *)p_buf_hdr->pOutputPortPrivate;
        ptr_extradatabuff->metadata_info.metabufaddr = (void *)p_extradata;
        ptr_extradatabuff->metadata_info.size = drv_ctx.extradata_info.buffer_size;
 }
 return;
}
