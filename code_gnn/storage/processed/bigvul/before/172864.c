bt_status_t btif_storage_load_autopair_device_list() {
 if (btif_config_has_section(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST)) {
 return BT_STATUS_SUCCESS;
 }

 static const char *key_names[] = {
        BTIF_STORAGE_KEY_AUTOPAIR_BLACKLIST_ADDR,
        BTIF_STORAGE_KEY_AUTOPAIR_BLACKLIST_EXACTNAME,
        BTIF_STORAGE_KEY_AUTOPAIR_FIXPIN_KBLIST,
        BTIF_STORAGE_KEY_AUTOPAIR_BLACKLIST_PARTIALNAME,
        BTIF_STORAGE_KEY_AUTOPAIR_DYNAMIC_BLACKLIST_ADDR,
 };

 config_t *config = config_new(BTIF_AUTO_PAIR_CONF_FILE);
 if (!config) {
        LOG_ERROR("%s failed to open auto pair blacklist conf file '%s'.", __func__, BTIF_AUTO_PAIR_CONF_FILE);
 return BT_STATUS_FAIL;
 }

 for (size_t i = 0; i < ARRAY_SIZE(key_names); ++i) {
 const char *value = config_get_string(config, CONFIG_DEFAULT_SECTION, key_names[i], NULL);
 if (value) {
            btif_config_set_str(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST, key_names[i], value);
 }
 }

    config_free(config);
 return BT_STATUS_SUCCESS;
}
