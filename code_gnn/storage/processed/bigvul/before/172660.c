status_t MediaPlayer::getDuration_l(int *msec)
{
    ALOGV("getDuration_l");
 bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED |
            MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
 if (mPlayer != 0 && isValidState) {
 int durationMs;
 status_t ret = mPlayer->getDuration(&durationMs);

 if (ret != OK) {
            durationMs = -1;
            ret = OK;
 }

 if (msec) {
 *msec = durationMs;
 }
 return ret;
 }
    ALOGE("Attempt to call getDuration without a valid mediaplayer");
 return INVALID_OPERATION;
}
