bt_status_t btif_storage_add_device_to_autopair_blacklist(bt_bdaddr_t *remote_bd_addr)
{
 int ret;
 bdstr_t bdstr;
 char linebuf[BTIF_STORAGE_MAX_LINE_SZ+20];
 char input_value [20];

    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
    strlcpy(input_value, (char*)bdstr, sizeof(input_value));
    strlcat(input_value,BTIF_AUTO_PAIR_CONF_VALUE_SEPARATOR, sizeof(input_value));

 int line_size = sizeof(linebuf);
 if(btif_config_get_str(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST,
                            BTIF_STORAGE_KEY_AUTOPAIR_DYNAMIC_BLACKLIST_ADDR, linebuf, &line_size))
 {
  
        strncat (linebuf, input_value, BTIF_STORAGE_MAX_LINE_SZ);
 }
 else
 {
        strncpy( linebuf,input_value, BTIF_STORAGE_MAX_LINE_SZ);
 }

  
    ret = btif_config_set_str(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST,
                        BTIF_STORAGE_KEY_AUTOPAIR_DYNAMIC_BLACKLIST_ADDR, linebuf);

 return ret ? BT_STATUS_SUCCESS:BT_STATUS_FAIL;
}
