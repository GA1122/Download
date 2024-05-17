int init_uinput (void)
{
 char *name = "AVRCP";

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
    uinput_fd = uinput_create(name);
 if (uinput_fd < 0) {
        BTIF_TRACE_ERROR("%s AVRCP: Failed to initialize uinput for %s (%d)",
                          __FUNCTION__, name, uinput_fd);
 } else {
        BTIF_TRACE_DEBUG("%s AVRCP: Initialized uinput for %s (fd=%d)",
                          __FUNCTION__, name, uinput_fd);
 }
 return uinput_fd;
}
