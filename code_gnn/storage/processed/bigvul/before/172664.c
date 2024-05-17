status_t MediaPlayer::getSyncSettings(
 AVSyncSettings* sync  , float* videoFps  )
{
 Mutex::Autolock _l(mLock);
 if (mPlayer == 0) return INVALID_OPERATION;
 return mPlayer->getSyncSettings(sync, videoFps);
}
