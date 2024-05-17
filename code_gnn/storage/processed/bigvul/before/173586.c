void NuPlayer::GenericSource::notifyBufferingUpdate(int32_t percentage) {
 if (percentage < mPrevBufferPercentage) {
        percentage = mPrevBufferPercentage;
 } else if (percentage > 100) {
        percentage = 100;
 }

    mPrevBufferPercentage = percentage;

    ALOGV("notifyBufferingUpdate: buffering %d%%", percentage);

    sp<AMessage> msg = dupNotify();
    msg->setInt32("what", kWhatBufferingUpdate);
    msg->setInt32("percentage", percentage);
    msg->post();
}
