int Visualizer_process(
 effect_handle_t self,audio_buffer_t *inBuffer, audio_buffer_t *outBuffer)
{
 VisualizerContext * pContext = (VisualizerContext *)self;

 if (pContext == NULL) {
 return -EINVAL;
 }

 if (inBuffer == NULL || inBuffer->raw == NULL ||
        outBuffer == NULL || outBuffer->raw == NULL ||
        inBuffer->frameCount != outBuffer->frameCount ||
        inBuffer->frameCount == 0) {
 return -EINVAL;
 }

 if (pContext->mMeasurementMode & MEASUREMENT_MODE_PEAK_RMS) {
 uint32_t inIdx;
 int16_t maxSample = 0;
 float rmsSqAcc = 0;
 for (inIdx = 0 ; inIdx < inBuffer->frameCount * pContext->mChannelCount ; inIdx++) {
 if (inBuffer->s16[inIdx] > maxSample) {
                maxSample = inBuffer->s16[inIdx];
 } else if (-inBuffer->s16[inIdx] > maxSample) {
                maxSample = -inBuffer->s16[inIdx];
 }
            rmsSqAcc += (inBuffer->s16[inIdx] * inBuffer->s16[inIdx]);
 }
        pContext->mPastMeasurements[pContext->mMeasurementBufferIdx].mPeakU16 = (uint16_t)maxSample;
        pContext->mPastMeasurements[pContext->mMeasurementBufferIdx].mRmsSquared =
                rmsSqAcc / (inBuffer->frameCount * pContext->mChannelCount);
        pContext->mPastMeasurements[pContext->mMeasurementBufferIdx].mIsValid = true;
 if (++pContext->mMeasurementBufferIdx >= pContext->mMeasurementWindowSizeInBuffers) {
            pContext->mMeasurementBufferIdx = 0;
 }
 }

 int32_t shift;

 if (pContext->mScalingMode == VISUALIZER_SCALING_MODE_NORMALIZED) {
        shift = 32;
 int len = inBuffer->frameCount * 2;
 for (int i = 0; i < len; i++) {
 int32_t smp = inBuffer->s16[i];
 if (smp < 0) smp = -smp - 1;  
 int32_t clz = __builtin_clz(smp);
 if (shift > clz) shift = clz;
 }
        shift = 25 - shift;
 if (shift < 3) {
            shift = 3;
 }
        shift++;
 } else {
        assert(pContext->mScalingMode == VISUALIZER_SCALING_MODE_AS_PLAYED);
        shift = 9;
 }

 uint32_t captIdx;
 uint32_t inIdx;
 uint8_t *buf = pContext->mCaptureBuf;
 for (inIdx = 0, captIdx = pContext->mCaptureIdx;
         inIdx < inBuffer->frameCount;
         inIdx++, captIdx++) {
 if (captIdx >= CAPTURE_BUF_SIZE) {
            captIdx = 0;
 }
 int32_t smp = inBuffer->s16[2 * inIdx] + inBuffer->s16[2 * inIdx + 1];
        smp = smp >> shift;
        buf[captIdx] = ((uint8_t)smp)^0x80;
 }

    pContext->mCaptureIdx = captIdx;
 if (clock_gettime(CLOCK_MONOTONIC, &pContext->mBufferUpdateTime) < 0) {
        pContext->mBufferUpdateTime.tv_sec = 0;
 }

 if (inBuffer->raw != outBuffer->raw) {
 if (pContext->mConfig.outputCfg.accessMode == EFFECT_BUFFER_ACCESS_ACCUMULATE) {
 for (size_t i = 0; i < outBuffer->frameCount*2; i++) {
                outBuffer->s16[i] = clamp16(outBuffer->s16[i] + inBuffer->s16[i]);
 }
 } else {
            memcpy(outBuffer->raw, inBuffer->raw, outBuffer->frameCount * 2 * sizeof(int16_t));
 }
 }
 if (pContext->mState != VISUALIZER_STATE_ACTIVE) {
 return -ENODATA;
 }
 return 0;
}  
