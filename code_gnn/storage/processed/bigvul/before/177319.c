status_t ACodec::LoadedState::setupInputSurface() {
 status_t err = OK;

 if (mCodec->mRepeatFrameDelayUs > 0ll) {
        err = mCodec->mOMX->setInternalOption(
                mCodec->mNode,
                kPortIndexInput,
                IOMX::INTERNAL_OPTION_REPEAT_PREVIOUS_FRAME_DELAY,
 &mCodec->mRepeatFrameDelayUs,
 sizeof(mCodec->mRepeatFrameDelayUs));

 if (err != OK) {
            ALOGE("[%s] Unable to configure option to repeat previous "
 "frames (err %d)",
                  mCodec->mComponentName.c_str(),
                  err);
 return err;
 }
 }

 if (mCodec->mMaxPtsGapUs > 0ll) {
        err = mCodec->mOMX->setInternalOption(
                mCodec->mNode,
                kPortIndexInput,
                IOMX::INTERNAL_OPTION_MAX_TIMESTAMP_GAP,
 &mCodec->mMaxPtsGapUs,
 sizeof(mCodec->mMaxPtsGapUs));

 if (err != OK) {
            ALOGE("[%s] Unable to configure max timestamp gap (err %d)",
                    mCodec->mComponentName.c_str(),
                    err);
 return err;
 }
 }

 if (mCodec->mMaxFps > 0) {
        err = mCodec->mOMX->setInternalOption(
                mCodec->mNode,
                kPortIndexInput,
                IOMX::INTERNAL_OPTION_MAX_FPS,
 &mCodec->mMaxFps,
 sizeof(mCodec->mMaxFps));

 if (err != OK) {
            ALOGE("[%s] Unable to configure max fps (err %d)",
                    mCodec->mComponentName.c_str(),
                    err);
 return err;
 }
 }

 if (mCodec->mTimePerCaptureUs > 0ll
 && mCodec->mTimePerFrameUs > 0ll) {
 int64_t timeLapse[2];
        timeLapse[0] = mCodec->mTimePerFrameUs;
        timeLapse[1] = mCodec->mTimePerCaptureUs;
        err = mCodec->mOMX->setInternalOption(
                mCodec->mNode,
                kPortIndexInput,
                IOMX::INTERNAL_OPTION_TIME_LAPSE,
 &timeLapse[0],
 sizeof(timeLapse));

 if (err != OK) {
            ALOGE("[%s] Unable to configure time lapse (err %d)",
                    mCodec->mComponentName.c_str(),
                    err);
 return err;
 }
 }

 if (mCodec->mCreateInputBuffersSuspended) {
 bool suspend = true;
        err = mCodec->mOMX->setInternalOption(
                mCodec->mNode,
                kPortIndexInput,
                IOMX::INTERNAL_OPTION_SUSPEND,
 &suspend,
 sizeof(suspend));

 if (err != OK) {
            ALOGE("[%s] Unable to configure option to suspend (err %d)",
                  mCodec->mComponentName.c_str(),
                  err);
 return err;
 }
 }

 uint32_t usageBits;
 if (mCodec->mOMX->getParameter(
            mCodec->mNode, (OMX_INDEXTYPE)OMX_IndexParamConsumerUsageBits,
 &usageBits, sizeof(usageBits)) == OK) {
        mCodec->mInputFormat->setInt32(
 "using-sw-read-often", !!(usageBits & GRALLOC_USAGE_SW_READ_OFTEN));
 }

 return OK;
}
