status_t MediaPlayerService::Client::setParameter(int key, const Parcel &request) {
    ALOGV("[%d] setParameter(%d)", mConnId, key);
 switch (key) {
 case KEY_PARAMETER_AUDIO_ATTRIBUTES:
 {
 Mutex::Autolock l(mLock);
 return setAudioAttributes_l(request);
 }
 default:
        sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) { return UNKNOWN_ERROR; }
 return p->setParameter(key, request);
 }
}
