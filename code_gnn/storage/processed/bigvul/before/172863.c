BOOLEAN  btif_storage_is_device_autopair_blacklisted(bt_bdaddr_t *remote_bd_addr)
{
 char *token;
 bdstr_t bdstr;
 char *dev_name_str;
 char value[BTIF_STORAGE_MAX_LINE_SZ];
 int value_size = sizeof(value);

    bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));

  
    bdstr[8] = '\0';

 if(btif_config_get_str(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST,
                BTIF_STORAGE_KEY_AUTOPAIR_BLACKLIST_ADDR, value, &value_size))
 {
 if (strcasestr(value,bdstr) != NULL)
 return TRUE;
 }

    dev_name_str = BTM_SecReadDevName((remote_bd_addr->address));

 if (dev_name_str != NULL)
 {
        value_size = sizeof(value);
 if(btif_config_get_str(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST,
                    BTIF_STORAGE_KEY_AUTOPAIR_BLACKLIST_EXACTNAME, value, &value_size))
 {
 if (strstr(value,dev_name_str) != NULL)
 return TRUE;
 }
        value_size = sizeof(value);
 if(btif_config_get_str(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST,
                    BTIF_STORAGE_KEY_AUTOPAIR_BLACKLIST_PARTIALNAME, value, &value_size))
 {
            token = strtok(value, BTIF_AUTO_PAIR_CONF_VALUE_SEPARATOR);
 while (token != NULL)
 {
 if (strstr(dev_name_str, token) != NULL)
 return TRUE;

                token = strtok(NULL, BTIF_AUTO_PAIR_CONF_VALUE_SEPARATOR);
 }
 }
 }
 if(btif_config_get_str(BTIF_STORAGE_PATH_AUTOPAIR_BLACKLIST,
                BTIF_STORAGE_KEY_AUTOPAIR_DYNAMIC_BLACKLIST_ADDR, value, &value_size))
 {
 if (strstr(value,bdstr) != NULL)
 return TRUE;
 }
 return FALSE;
}
