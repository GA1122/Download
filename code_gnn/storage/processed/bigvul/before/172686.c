status_t MediaPlayer::setParameter(int key, const Parcel& request)
{
    ALOGV("MediaPlayer::setParameter(%d)", key);
 status_t status = INVALID_OPERATION;
 Mutex::Autolock _l(mLock);
 if (checkStateForKeySet_l(key) != OK) {
 return status;
 }
 switch (key) {
 case KEY_PARAMETER_AUDIO_ATTRIBUTES:
 if (mAudioAttributesParcel != NULL) { delete mAudioAttributesParcel; };
        mAudioAttributesParcel = new Parcel();
        mAudioAttributesParcel->appendFrom(&request, 0, request.dataSize());
        status = OK;
 break;
 default:
        ALOGV_IF(mPlayer == NULL, "setParameter: no active player");
 break;
 }

 if (mPlayer != NULL) {
        status = mPlayer->setParameter(key, request);
 }
 return status;
}
