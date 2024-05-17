status_t MediaPlayerService::Client::getDuration(int *msec)
{
    ALOGV("getDuration");
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 status_t ret = p->getDuration(msec);
 if (ret == NO_ERROR) {
        ALOGV("[%d] getDuration = %d", mConnId, *msec);
 } else {
        ALOGE("getDuration returned %d", ret);
 }
 return ret;
}
