status_t NuPlayer::GenericSource::setDataSource(
 int fd, int64_t offset, int64_t length) {
    resetDataSource();

    mFd = dup(fd);
    mOffset = offset;
    mLength = length;

 return OK;
}
