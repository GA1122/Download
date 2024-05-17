static int config_clear(void) {
    LOG_INFO("%s", __func__);
 return btif_config_clear();
}
