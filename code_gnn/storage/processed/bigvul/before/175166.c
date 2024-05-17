void NuPlayer::GenericSource::onReadBuffer(sp<AMessage> msg) {
 int32_t tmpType;
    CHECK(msg->findInt32("trackType", &tmpType));
    media_track_type trackType = (media_track_type)tmpType;
 {
 Mutex::Autolock _l(mReadBufferLock);
        mPendingReadBufferTypes &= ~(1 << trackType);
 }
    readBuffer(trackType);
}
