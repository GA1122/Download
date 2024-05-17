status_t MediaPlayer::attachAuxEffect(int effectId)
{
    ALOGV("MediaPlayer::attachAuxEffect(%d)", effectId);
 Mutex::Autolock _l(mLock);
 if (mPlayer == 0 ||
 (mCurrentState & MEDIA_PLAYER_IDLE) ||
 (mCurrentState == MEDIA_PLAYER_STATE_ERROR )) {
        ALOGE("attachAuxEffect called in state %d", mCurrentState);
 return INVALID_OPERATION;
 }

 return mPlayer->attachAuxEffect(effectId);
}
