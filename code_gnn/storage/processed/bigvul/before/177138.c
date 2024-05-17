OMX_ERRORTYPE SoftAVC::setEncodeArgs(
 ive_video_encode_ip_t *ps_encode_ip,
 ive_video_encode_op_t *ps_encode_op,
        OMX_BUFFERHEADERTYPE *inputBufferHeader,
        OMX_BUFFERHEADERTYPE *outputBufferHeader) {
 iv_raw_buf_t *ps_inp_raw_buf;
 const uint8_t *source;
    UWORD8 *pu1_buf;

    ps_inp_raw_buf = &ps_encode_ip->s_inp_buf;
    ps_encode_ip->s_out_buf.pv_buf = outputBufferHeader->pBuffer;
    ps_encode_ip->s_out_buf.u4_bytes = 0;
    ps_encode_ip->s_out_buf.u4_bufsize = outputBufferHeader->nAllocLen;
    ps_encode_ip->u4_size = sizeof(ive_video_encode_ip_t);
    ps_encode_op->u4_size = sizeof(ive_video_encode_op_t);

    ps_encode_ip->e_cmd = IVE_CMD_VIDEO_ENCODE;
    ps_encode_ip->pv_bufs = NULL;
    ps_encode_ip->pv_mb_info = NULL;
    ps_encode_ip->pv_pic_info = NULL;
    ps_encode_ip->u4_mb_info_type = 0;
    ps_encode_ip->u4_pic_info_type = 0;
    ps_encode_op->s_out_buf.pv_buf = NULL;

  
    ps_inp_raw_buf->e_color_fmt = mIvVideoColorFormat;
    source = NULL;
 if ((inputBufferHeader != NULL) && inputBufferHeader->nFilledLen) {
        source = inputBufferHeader->pBuffer + inputBufferHeader->nOffset;

 if (mInputDataIsMeta) {
 uint8_t *conversionBuffer = NULL;
 for (size_t i = 0; i < MAX_CONVERSION_BUFFERS; i++) {
 if (mConversionBuffersFree[i]) {
                    mConversionBuffersFree[i] = 0;
                    conversionBuffer = mConversionBuffers[i];
 break;
 }
 }

 if (NULL == conversionBuffer) {
                ALOGE("No free buffers to hold conversion data");
 return OMX_ErrorUndefined;
 }

            source = extractGraphicBuffer(
                    conversionBuffer, (mWidth * mHeight * 3 / 2), source,
                    inputBufferHeader->nFilledLen, mWidth, mHeight);

 if (source == NULL) {
                ALOGE("Error in extractGraphicBuffer");
                notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return OMX_ErrorUndefined;
 }
 }
        ps_encode_ip->u4_is_last = 0;
        ps_encode_ip->u4_timestamp_high = (inputBufferHeader->nTimeStamp) >> 32;
        ps_encode_ip->u4_timestamp_low = (inputBufferHeader->nTimeStamp) & 0xFFFFFFFF;
 }
 else {
 if (mSawInputEOS){
            ps_encode_ip->u4_is_last = 1;
 }
        memset(ps_inp_raw_buf, 0, sizeof(iv_raw_buf_t));
        ps_inp_raw_buf->e_color_fmt = mIvVideoColorFormat;
        ps_inp_raw_buf->u4_size = sizeof(iv_raw_buf_t);
 return OMX_ErrorNone;
 }

    pu1_buf = (UWORD8 *)source;
 switch (mIvVideoColorFormat) {
 case IV_YUV_420P:
 {
            ps_inp_raw_buf->apv_bufs[0] = pu1_buf;
            pu1_buf += (mStride) * mHeight;
            ps_inp_raw_buf->apv_bufs[1] = pu1_buf;
            pu1_buf += (mStride / 2) * mHeight / 2;
            ps_inp_raw_buf->apv_bufs[2] = pu1_buf;

            ps_inp_raw_buf->au4_wd[0] = mWidth;
            ps_inp_raw_buf->au4_wd[1] = mWidth / 2;
            ps_inp_raw_buf->au4_wd[2] = mWidth / 2;

            ps_inp_raw_buf->au4_ht[0] = mHeight;
            ps_inp_raw_buf->au4_ht[1] = mHeight / 2;
            ps_inp_raw_buf->au4_ht[2] = mHeight / 2;

            ps_inp_raw_buf->au4_strd[0] = mStride;
            ps_inp_raw_buf->au4_strd[1] = (mStride / 2);
            ps_inp_raw_buf->au4_strd[2] = (mStride / 2);
 break;
 }

 case IV_YUV_422ILE:
 {
            ps_inp_raw_buf->apv_bufs[0] = pu1_buf;
            ps_inp_raw_buf->au4_wd[0] = mWidth * 2;
            ps_inp_raw_buf->au4_ht[0] = mHeight;
            ps_inp_raw_buf->au4_strd[0] = mStride * 2;
 break;
 }

 case IV_YUV_420SP_UV:
 case IV_YUV_420SP_VU:
 default:
 {
            ps_inp_raw_buf->apv_bufs[0] = pu1_buf;
            pu1_buf += (mStride) * mHeight;
            ps_inp_raw_buf->apv_bufs[1] = pu1_buf;

            ps_inp_raw_buf->au4_wd[0] = mWidth;
            ps_inp_raw_buf->au4_wd[1] = mWidth;

            ps_inp_raw_buf->au4_ht[0] = mHeight;
            ps_inp_raw_buf->au4_ht[1] = mHeight / 2;

            ps_inp_raw_buf->au4_strd[0] = mStride;
            ps_inp_raw_buf->au4_strd[1] = mStride;
 break;
 }
 }
 return OMX_ErrorNone;
}
