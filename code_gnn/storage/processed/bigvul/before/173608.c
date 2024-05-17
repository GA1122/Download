void NuPlayer::GenericSource::sendCacheStats() {
 int32_t kbps = 0;
 status_t err = UNKNOWN_ERROR;

 if (mWVMExtractor != NULL) {
        err = mWVMExtractor->getEstimatedBandwidthKbps(&kbps);
 } else if (mCachedSource != NULL) {
        err = mCachedSource->getEstimatedBandwidthKbps(&kbps);
 }

 if (err == OK) {
        sp<AMessage> notify = dupNotify();
        notify->setInt32("what", kWhatCacheStats);
        notify->setInt32("bandwidth", kbps);
        notify->post();
 }
}
