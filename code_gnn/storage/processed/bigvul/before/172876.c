static int prop2cfg(bt_bdaddr_t *remote_bd_addr, bt_property_t *prop)
{
 bdstr_t bdstr = {0};
 if(remote_bd_addr)
        bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
    BTIF_TRACE_DEBUG("in, bd addr:%s, prop type:%d, len:%d", bdstr, prop->type, prop->len);
 char value[1024];
 if(prop->len <= 0 || prop->len > (int)sizeof(value) - 1)
 {
        BTIF_TRACE_ERROR("property type:%d, len:%d is invalid", prop->type, prop->len);
 return FALSE;
 }
 switch(prop->type)
 {
 case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
            btif_config_set_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_DEVTIME, (int)time(NULL));
 break;
 case BT_PROPERTY_BDNAME:
            strncpy(value, (char*)prop->val, prop->len);
            value[prop->len]='\0';
 if(remote_bd_addr)
                btif_config_set_str(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_NAME, value);
 else btif_config_set_str("Adapter",
                                BTIF_STORAGE_KEY_ADAPTER_NAME, value);
  
            btif_config_save();
 break;
 case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
            strncpy(value, (char*)prop->val, prop->len);
            value[prop->len]='\0';
            btif_config_set_str(bdstr, BTIF_STORAGE_PATH_REMOTE_ALIASE, value);
  
            btif_config_save();
 break;
 case BT_PROPERTY_ADAPTER_SCAN_MODE:
            btif_config_set_int("Adapter",
                                BTIF_STORAGE_KEY_ADAPTER_SCANMODE, *(int*)prop->val);
 break;
 case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
            btif_config_set_int("Adapter",
                                BTIF_STORAGE_KEY_ADAPTER_DISC_TIMEOUT, *(int*)prop->val);
 break;
 case BT_PROPERTY_CLASS_OF_DEVICE:
            btif_config_set_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_DEVCLASS, *(int*)prop->val);
 break;
 case BT_PROPERTY_TYPE_OF_DEVICE:
            btif_config_set_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_DEVTYPE, *(int*)prop->val);
 break;
 case BT_PROPERTY_UUIDS:
 {
 uint32_t i;
 char buf[64];
            value[0] = 0;
 for (i=0; i < (prop->len)/sizeof(bt_uuid_t); i++)
 {
 bt_uuid_t *p_uuid = (bt_uuid_t*)prop->val + i;
                memset(buf, 0, sizeof(buf));
                uuid_to_string_legacy(p_uuid, buf);
                strcat(value, buf);
                strcat(value, " ");
 }
            btif_config_set_str(bdstr, BTIF_STORAGE_PATH_REMOTE_SERVICE, value);
            btif_config_save();
 break;
 }
 case BT_PROPERTY_REMOTE_VERSION_INFO:
 {
 bt_remote_version_t *info = (bt_remote_version_t *)prop->val;

 if (!info)
 return FALSE;

            btif_config_set_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_VER_MFCT, info->manufacturer);
            btif_config_set_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_VER_VER, info->version);
            btif_config_set_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_VER_SUBVER, info->sub_ver);
            btif_config_save();
 } break;

 default:
             BTIF_TRACE_ERROR("Unknow prop type:%d", prop->type);
 return FALSE;
 }
 return TRUE;
}
