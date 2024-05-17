void SoftHEVC::onPortFlushCompleted(OMX_U32 portIndex) {
  
 if (kOutputPortIndex == portIndex) {
        setFlushMode();

  
 uint32_t displayStride = outputBufferWidth();
 uint32_t displayHeight = outputBufferHeight();

 uint32_t bufferSize = displayStride * displayHeight * 3 / 2;
        mFlushOutBuffer = (uint8_t *)memalign(128, bufferSize);
 if (NULL == mFlushOutBuffer) {
            ALOGE("Could not allocate flushOutputBuffer of size %zu", bufferSize);
 return;
 }

 while (true) {
 ivd_video_decode_ip_t s_dec_ip;
 ivd_video_decode_op_t s_dec_op;
            IV_API_CALL_STATUS_T status;
 size_t sizeY, sizeUV;

            setDecodeArgs(&s_dec_ip, &s_dec_op, NULL, NULL, 0);

            status = ivdec_api_function(mCodecCtx, (void *)&s_dec_ip,
 (void *)&s_dec_op);
 if (0 == s_dec_op.u4_output_present) {
                resetPlugin();
 break;
 }
 }

 if (mFlushOutBuffer) {
            free(mFlushOutBuffer);
            mFlushOutBuffer = NULL;
 }

 }
}
