status_t MediaPlayer::setAudioStreamType(audio_stream_type_t type)
{
    ALOGV("MediaPlayer::setAudioStreamType");
 Mutex::Autolock _l(mLock);
 if (mStreamType == type) return NO_ERROR;
 if (mCurrentState & ( MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED |
                MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE ) ) {
        ALOGE("setAudioStream called in state %d", mCurrentState);
 return INVALID_OPERATION;
 }
    mStreamType = type;
 return OK;
}
