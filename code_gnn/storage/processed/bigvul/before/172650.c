status_t MediaPlayer::attachNewPlayer(const sp<IMediaPlayer>& player)
{
 status_t err = UNKNOWN_ERROR;
    sp<IMediaPlayer> p;
 {  
 Mutex::Autolock _l(mLock);

 if ( !( (mCurrentState & MEDIA_PLAYER_IDLE) ||
 (mCurrentState == MEDIA_PLAYER_STATE_ERROR ) ) ) {
            ALOGE("attachNewPlayer called in state %d", mCurrentState);
 return INVALID_OPERATION;
 }

        clear_l();
        p = mPlayer;
        mPlayer = player;
 if (player != 0) {
            mCurrentState = MEDIA_PLAYER_INITIALIZED;
            err = NO_ERROR;
 } else {
            ALOGE("Unable to create media player");
 }
 }

 if (p != 0) {
        p->disconnect();
 }

 return err;
}
