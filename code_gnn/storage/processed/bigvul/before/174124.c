void SoftAMR::onPortFlushCompleted(OMX_U32 portIndex) {
    ALOGV("onPortFlushCompleted portindex %d, resetting frame ", portIndex);
 if (portIndex == 0) {
 if (mMode == MODE_NARROW) {
 Speech_Decode_Frame_reset(mState);
 } else {
           pvDecoder_AmrWb_Reset(mState, 0  );
 }
 }
}
