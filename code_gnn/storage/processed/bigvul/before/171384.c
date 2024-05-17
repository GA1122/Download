status_t OMXCodec::waitForBufferFilled_l() {

 if (mIsEncoder) {
 return mBufferFilled.wait(mLock);
 }
 status_t err = mBufferFilled.waitRelative(mLock, kBufferFilledEventTimeOutNs);
 if (err != OK) {
        CODEC_LOGE("Timed out waiting for output buffers: %zu/%zu",
            countBuffersWeOwn(mPortBuffers[kPortIndexInput]),
            countBuffersWeOwn(mPortBuffers[kPortIndexOutput]));
 }
 return err;
}
