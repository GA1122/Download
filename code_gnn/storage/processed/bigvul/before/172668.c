bool MediaPlayer::isLooping() {
    ALOGV("isLooping");
 Mutex::Autolock _l(mLock);
 if (mPlayer != 0) {
 return mLoop;
 }
    ALOGV("isLooping: no active player");
 return false;
}
