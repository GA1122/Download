void uhid_set_non_blocking(int fd)
{
 int opts = fcntl(fd, F_GETFL);
 if (opts < 0)
        APPL_TRACE_ERROR("%s() Getting flags failed (%s)", __func__, strerror(errno));

    opts |= O_NONBLOCK;

 if (fcntl(fd, F_SETFL, opts) < 0)
        APPL_TRACE_EVENT("%s() Setting non-blocking flag failed (%s)", __func__, strerror(errno));
}
