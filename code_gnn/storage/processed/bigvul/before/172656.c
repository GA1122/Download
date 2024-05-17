int MediaPlayer::getAudioSessionId()
{
 Mutex::Autolock _l(mLock);
 return mAudioSessionId;
}
