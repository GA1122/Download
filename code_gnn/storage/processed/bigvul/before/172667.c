status_t MediaPlayer::invoke(const Parcel& request, Parcel *reply)
{
 Mutex::Autolock _l(mLock);
 const bool hasBeenInitialized =
 (mCurrentState != MEDIA_PLAYER_STATE_ERROR) &&
 ((mCurrentState & MEDIA_PLAYER_IDLE) != MEDIA_PLAYER_IDLE);
 if ((mPlayer != NULL) && hasBeenInitialized) {
        ALOGV("invoke %zu", request.dataSize());
 return  mPlayer->invoke(request, reply);
 }
    ALOGE("invoke failed: wrong state %X", mCurrentState);
 return INVALID_OPERATION;
}
