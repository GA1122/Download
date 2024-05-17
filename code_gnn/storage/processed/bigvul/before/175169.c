void NuPlayer::GenericSource::postReadBuffer(media_track_type trackType) {
 Mutex::Autolock _l(mReadBufferLock);

 if ((mPendingReadBufferTypes & (1 << trackType)) == 0) {
        mPendingReadBufferTypes |= (1 << trackType);
        sp<AMessage> msg = new AMessage(kWhatReadBuffer, id());
        msg->setInt32("trackType", trackType);
        msg->post();
 }
}
