status_t MediaPlayer::getPlaybackSettings(AudioPlaybackRate* rate  )
{
 Mutex::Autolock _l(mLock);
 if (mPlayer == 0) return INVALID_OPERATION;
 return mPlayer->getPlaybackSettings(rate);
}
