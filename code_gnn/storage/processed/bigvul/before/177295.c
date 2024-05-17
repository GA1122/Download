void ACodec::sendFormatChange(const sp<AMessage> &reply) {
    sp<AMessage> notify = mBaseOutputFormat->dup();
    notify->setInt32("what", kWhatOutputFormatChanged);

 if (getPortFormat(kPortIndexOutput, notify) != OK) {
        ALOGE("[%s] Failed to get port format to send format change", mComponentName.c_str());
 return;
 }

 AString mime;
    CHECK(notify->findString("mime", &mime));

 int32_t left, top, right, bottom;
 if (mime == MEDIA_MIMETYPE_VIDEO_RAW &&
        mNativeWindow != NULL &&
        notify->findRect("crop", &left, &top, &right, &bottom)) {
        reply->setRect("crop", left, top, right + 1, bottom + 1);
 } else if (mime == MEDIA_MIMETYPE_AUDIO_RAW &&
 (mEncoderDelay || mEncoderPadding)) {
 int32_t channelCount;
        CHECK(notify->findInt32("channel-count", &channelCount));
 size_t frameSize = channelCount * sizeof(int16_t);
 if (mSkipCutBuffer != NULL) {
 size_t prevbufsize = mSkipCutBuffer->size();
 if (prevbufsize != 0) {
                ALOGW("Replacing SkipCutBuffer holding %zu bytes", prevbufsize);
 }
 }
        mSkipCutBuffer = new SkipCutBuffer(
                mEncoderDelay * frameSize,
                mEncoderPadding * frameSize);
 }

    notify->post();

    mSentFormat = true;
}
