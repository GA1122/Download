status_t MediaPlayer::setAudioSessionId(int sessionId)
{
    ALOGV("MediaPlayer::setAudioSessionId(%d)", sessionId);
 Mutex::Autolock _l(mLock);
 if (!(mCurrentState & MEDIA_PLAYER_IDLE)) {
        ALOGE("setAudioSessionId called in state %d", mCurrentState);
 return INVALID_OPERATION;
 }
 if (sessionId < 0) {
 return BAD_VALUE;
 }
 if (sessionId != mAudioSessionId) {
 AudioSystem::acquireAudioSessionId(sessionId, -1);
 AudioSystem::releaseAudioSessionId(mAudioSessionId, -1);
        mAudioSessionId = sessionId;
 }
 return NO_ERROR;
}
