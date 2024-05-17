void NuPlayer::GenericSource::stopBufferingIfNecessary() {
    ALOGV("stopBufferingIfNecessary: mPrepareBuffering=%d, mBuffering=%d",
            mPrepareBuffering, mBuffering);

 if (mPrepareBuffering) {
        mPrepareBuffering = false;
        notifyPrepared();
 return;
 }

 if (mBuffering) {
        mBuffering = false;

        sendCacheStats();

        sp<AMessage> notify = dupNotify();
        notify->setInt32("what", kWhatResumeOnBufferingEnd);
        notify->post();
 }
}
