status_t MetadataRetrieverClient::setDataSource(int fd, int64_t offset, int64_t length)
{
    ALOGV("setDataSource fd=%d, offset=%lld, length=%lld", fd, offset, length);
 Mutex::Autolock lock(mLock);
 struct stat sb;
 int ret = fstat(fd, &sb);
 if (ret != 0) {
        ALOGE("fstat(%d) failed: %d, %s", fd, ret, strerror(errno));
 return BAD_VALUE;
 }
    ALOGV("st_dev  = %llu", static_cast<uint64_t>(sb.st_dev));
    ALOGV("st_mode = %u", sb.st_mode);
    ALOGV("st_uid  = %lu", static_cast<unsigned long>(sb.st_uid));
    ALOGV("st_gid  = %lu", static_cast<unsigned long>(sb.st_gid));
    ALOGV("st_size = %llu", sb.st_size);

 if (offset >= sb.st_size) {
        ALOGE("offset (%lld) bigger than file size (%llu)", offset, sb.st_size);
 ::close(fd);
 return BAD_VALUE;
 }
 if (offset + length > sb.st_size) {
        length = sb.st_size - offset;
        ALOGV("calculated length = %lld", length);
 }

    player_type playerType =
 MediaPlayerFactory::getPlayerType(NULL  ,
                                          fd,
                                          offset,
                                          length);
    ALOGV("player type = %d", playerType);
    sp<MediaMetadataRetrieverBase> p = createRetriever(playerType);
 if (p == NULL) {
 ::close(fd);
 return NO_INIT;
 }
 status_t status = p->setDataSource(fd, offset, length);
 if (status == NO_ERROR) mRetriever = p;
 ::close(fd);
 return status;
}
