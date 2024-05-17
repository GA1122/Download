void bta_hh_co_destroy(int fd)
{
 struct uhid_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_DESTROY;
    uhid_write(fd, &ev);
    APPL_TRACE_DEBUG("%s: Closing fd=%d", __func__, fd);
    close(fd);
}
