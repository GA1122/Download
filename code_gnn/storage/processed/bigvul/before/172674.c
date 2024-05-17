status_t MediaPlayer::prepareAsync_l()
{
 if ( (mPlayer != 0) && ( mCurrentState & (MEDIA_PLAYER_INITIALIZED | MEDIA_PLAYER_STOPPED) ) ) {
 if (mAudioAttributesParcel != NULL) {
            mPlayer->setParameter(KEY_PARAMETER_AUDIO_ATTRIBUTES, *mAudioAttributesParcel);
 } else {
            mPlayer->setAudioStreamType(mStreamType);
 }
        mCurrentState = MEDIA_PLAYER_PREPARING;
 return mPlayer->prepareAsync();
 }
    ALOGE("prepareAsync called in state %d", mCurrentState);
 return INVALID_OPERATION;
}
