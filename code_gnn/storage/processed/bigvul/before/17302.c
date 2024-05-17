static uint64_t get_free_space_available(const char *path)
{
    struct statvfs stat;
    if (statvfs(path, &stat) != 0) {
        syslog(LOG_WARNING, "file-xfer: failed to get free space, statvfs error: %s",
               strerror(errno));
        return G_MAXUINT64;
    }
    return stat.f_bsize * stat.f_bavail;
}
