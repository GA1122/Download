status_t MediaPlayerService::Client::seekTo(int msec)
{
    ALOGV("[%d] seekTo(%d)", mConnId, msec);
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 return p->seekTo(msec);
}
