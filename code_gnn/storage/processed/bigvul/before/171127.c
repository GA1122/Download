MediaPlayerService::AudioOutput::AudioOutput(audio_session_t sessionId, int uid, int pid,
 const audio_attributes_t* attr)
 : mCallback(NULL),
      mCallbackCookie(NULL),
      mCallbackData(NULL),
      mStreamType(AUDIO_STREAM_MUSIC),
      mLeftVolume(1.0),
      mRightVolume(1.0),
      mPlaybackRate(AUDIO_PLAYBACK_RATE_DEFAULT),
      mSampleRateHz(0),
      mMsecsPerFrame(0),
      mFrameSize(0),
      mSessionId(sessionId),
      mUid(uid),
      mPid(pid),
      mSendLevel(0.0),
      mAuxEffectId(0),
      mFlags(AUDIO_OUTPUT_FLAG_NONE)
{
    ALOGV("AudioOutput(%d)", sessionId);
 if (attr != NULL) {
        mAttributes = (audio_attributes_t *) calloc(1, sizeof(audio_attributes_t));
 if (mAttributes != NULL) {
            memcpy(mAttributes, attr, sizeof(audio_attributes_t));
            mStreamType = audio_attributes_to_stream_type(attr);
 }
 } else {
        mAttributes = NULL;
 }

    setMinBufferCount();
}
