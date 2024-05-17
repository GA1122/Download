void MediaPlayer::disconnect()
{
    ALOGV("disconnect");
    sp<IMediaPlayer> p;
 {
 Mutex::Autolock _l(mLock);
        p = mPlayer;
        mPlayer.clear();
 }

 if (p != 0) {
        p->disconnect();
 }
}
