status_t MediaPlayer::setNextMediaPlayer(const sp<MediaPlayer>& next) {
 if (mPlayer == NULL) {
 return NO_INIT;
 }

 if (next != NULL && !(next->mCurrentState &
 (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE))) {
        ALOGE("next player is not prepared");
 return INVALID_OPERATION;
 }

 return mPlayer->setNextPlayer(next == NULL ? NULL : next->mPlayer);
}
