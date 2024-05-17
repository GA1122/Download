status_t MediaPlayerService::Client::getCurrentPosition(int *msec)
{
    ALOGV("getCurrentPosition");
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 status_t ret = p->getCurrentPosition(msec);
 if (ret == NO_ERROR) {
        ALOGV("[%d] getCurrentPosition = %d", mConnId, *msec);
 } else {
        ALOGE("getCurrentPosition returned %d", ret);
 }
 return ret;
}
