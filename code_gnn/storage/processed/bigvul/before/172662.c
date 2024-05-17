status_t MediaPlayer::getParameter(int key, Parcel *reply)
{
    ALOGV("MediaPlayer::getParameter(%d)", key);
 Mutex::Autolock _l(mLock);
 if (mPlayer != NULL) {
 return  mPlayer->getParameter(key, reply);
 }
    ALOGV("getParameter: no active player");
 return INVALID_OPERATION;
}
