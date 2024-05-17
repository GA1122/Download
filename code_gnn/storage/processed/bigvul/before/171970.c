static void pan_disable()
{
 if (btpan_cb.enabled)
 {
        btpan_cb.enabled = 0;
        BTA_PanDisable();
 if (btpan_cb.tap_fd != INVALID_FD)
 {
            btpan_tap_close(btpan_cb.tap_fd);
            btpan_cb.tap_fd = INVALID_FD;
 }
 }
}
