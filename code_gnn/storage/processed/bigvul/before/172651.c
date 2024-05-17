status_t MediaPlayer::checkStateForKeySet_l(int key)
{
 switch(key) {
 case KEY_PARAMETER_AUDIO_ATTRIBUTES:
 if (mCurrentState & ( MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED |
                MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE) ) {
            ALOGE("trying to set audio attributes called in state %d", mCurrentState);
 return INVALID_OPERATION;
 }
 break;
 default:
 break;
 }
 return OK;
}
