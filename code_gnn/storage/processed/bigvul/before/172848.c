static void timer_config_save_cb(UNUSED_ATTR void *data) {
  btif_config_write();
}
