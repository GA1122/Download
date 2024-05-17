status_t MediaPlayerService::Client::setDataSource(
 const sp<IMediaHTTPService> &httpService,
 const char *url,
 const KeyedVector<String8, String8> *headers)
{
    ALOGV("setDataSource(%s)", url);
 if (url == NULL)
 return UNKNOWN_ERROR;

 if ((strncmp(url, "http://", 7) == 0) ||
 (strncmp(url, "https://", 8) == 0) ||
 (strncmp(url, "rtsp://", 7) == 0)) {
 if (!checkPermission("android.permission.INTERNET")) {
 return PERMISSION_DENIED;
 }
 }

 if (strncmp(url, "content://", 10) == 0) {

 String16 url16(url);
 int fd = android::openContentProviderFile(url16);
 if (fd < 0)
 {
            ALOGE("Couldn't open fd for %s", url);
 return UNKNOWN_ERROR;
 }
        setDataSource(fd, 0, 0x7fffffffffLL);  
        close(fd);
 return mStatus;
 } else {
        player_type playerType = MediaPlayerFactory::getPlayerType(this, url);
        sp<MediaPlayerBase> p = setDataSource_pre(playerType);
 if (p == NULL) {
 return NO_INIT;
 }

        setDataSource_post(p, p->setDataSource(httpService, url, headers));
 return mStatus;
 }
}
