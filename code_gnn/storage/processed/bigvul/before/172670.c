void MediaPlayer::notify(int msg, int ext1, int ext2, const Parcel *obj)
{
    ALOGV("message received msg=%d, ext1=%d, ext2=%d", msg, ext1, ext2);
 bool send = true;
 bool locked = false;

 if (mLockThreadId != getThreadId()) {
        mLock.lock();
        locked = true;
 }

 if (!(msg == MEDIA_ERROR && mCurrentState == MEDIA_PLAYER_IDLE) && mPlayer == 0) {
        ALOGV("notify(%d, %d, %d) callback on disconnected mediaplayer", msg, ext1, ext2);
 if (locked) mLock.unlock();  
 return;
 }

 switch (msg) {
 case MEDIA_NOP:  
 break;
 case MEDIA_PREPARED:
        ALOGV("prepared");
        mCurrentState = MEDIA_PLAYER_PREPARED;
 if (mPrepareSync) {
            ALOGV("signal application thread");
            mPrepareSync = false;
            mPrepareStatus = NO_ERROR;
            mSignal.signal();
 }
 break;
 case MEDIA_PLAYBACK_COMPLETE:
        ALOGV("playback complete");
 if (mCurrentState == MEDIA_PLAYER_IDLE) {
            ALOGE("playback complete in idle state");
 }
 if (!mLoop) {
            mCurrentState = MEDIA_PLAYER_PLAYBACK_COMPLETE;
 }
 break;
 case MEDIA_ERROR:
        ALOGE("error (%d, %d)", ext1, ext2);
        mCurrentState = MEDIA_PLAYER_STATE_ERROR;
 if (mPrepareSync)
 {
            ALOGV("signal application thread");
            mPrepareSync = false;
            mPrepareStatus = ext1;
            mSignal.signal();
            send = false;
 }
 break;
 case MEDIA_INFO:
 if (ext1 != MEDIA_INFO_VIDEO_TRACK_LAGGING) {
            ALOGW("info/warning (%d, %d)", ext1, ext2);
 }
 break;
 case MEDIA_SEEK_COMPLETE:
        ALOGV("Received seek complete");
 if (mSeekPosition != mCurrentPosition) {
            ALOGV("Executing queued seekTo(%d)", mSeekPosition);
            mSeekPosition = -1;
            seekTo_l(mCurrentPosition);
 }
 else {
            ALOGV("All seeks complete - return to regularly scheduled program");
            mCurrentPosition = mSeekPosition = -1;
 }
 break;
 case MEDIA_BUFFERING_UPDATE:
        ALOGV("buffering %d", ext1);
 break;
 case MEDIA_SET_VIDEO_SIZE:
        ALOGV("New video size %d x %d", ext1, ext2);
        mVideoWidth = ext1;
        mVideoHeight = ext2;
 break;
 case MEDIA_TIMED_TEXT:
        ALOGV("Received timed text message");
 break;
 case MEDIA_SUBTITLE_DATA:
        ALOGV("Received subtitle data message");
 break;
 case MEDIA_META_DATA:
        ALOGV("Received timed metadata message");
 break;
 default:
        ALOGV("unrecognized message: (%d, %d, %d)", msg, ext1, ext2);
 break;
 }

    sp<MediaPlayerListener> listener = mListener;
 if (locked) mLock.unlock();

 if ((listener != 0) && send) {
 Mutex::Autolock _l(mNotifyLock);
        ALOGV("callback application");
        listener->notify(msg, ext1, ext2, obj);
        ALOGV("back from callback");
 }
}
