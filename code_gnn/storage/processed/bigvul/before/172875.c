static int cfg2prop(bt_bdaddr_t *remote_bd_addr, bt_property_t *prop)
{
 bdstr_t bdstr = {0};
 if(remote_bd_addr)
        bdaddr_to_string(remote_bd_addr, bdstr, sizeof(bdstr));
    BTIF_TRACE_DEBUG("in, bd addr:%s, prop type:%d, len:%d", bdstr, prop->type, prop->len);
 if(prop->len <= 0)
 {
        BTIF_TRACE_ERROR("property type:%d, len:%d is invalid", prop->type, prop->len);
 return FALSE;
 }
 int ret = FALSE;
 switch(prop->type)
 {
 case BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP:
 if(prop->len >= (int)sizeof(int))
                ret = btif_config_get_int(bdstr,
                                        BTIF_STORAGE_PATH_REMOTE_DEVTIME, (int*)prop->val);
 break;
 case BT_PROPERTY_BDNAME:
 {
 int len = prop->len;
 if(remote_bd_addr)
                ret = btif_config_get_str(bdstr,
                                        BTIF_STORAGE_PATH_REMOTE_NAME, (char*)prop->val, &len);
 else ret = btif_config_get_str("Adapter",
                                        BTIF_STORAGE_KEY_ADAPTER_NAME, (char*)prop->val, &len);
 if(ret && len && len <= prop->len)
                prop->len = len - 1;
 else
 {
                prop->len = 0;
                ret = FALSE;
 }
 break;
 }
 case BT_PROPERTY_REMOTE_FRIENDLY_NAME:
 {
 int len = prop->len;
            ret = btif_config_get_str(bdstr,
                                       BTIF_STORAGE_PATH_REMOTE_ALIASE, (char*)prop->val, &len);
 if(ret && len && len <= prop->len)
                prop->len = len - 1;
 else
 {
                prop->len = 0;
                ret = FALSE;
 }
 break;
 }
 case BT_PROPERTY_ADAPTER_SCAN_MODE:
 if(prop->len >= (int)sizeof(int))
                ret = btif_config_get_int("Adapter",
                                          BTIF_STORAGE_KEY_ADAPTER_SCANMODE, (int*)prop->val);
 break;
 case BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT:
 if(prop->len >= (int)sizeof(int))
                ret = btif_config_get_int("Adapter",
                                          BTIF_STORAGE_KEY_ADAPTER_DISC_TIMEOUT, (int*)prop->val);
 break;
 case BT_PROPERTY_CLASS_OF_DEVICE:
 if(prop->len >= (int)sizeof(int))
                ret = btif_config_get_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_DEVCLASS, (int*)prop->val);
 break;
 case BT_PROPERTY_TYPE_OF_DEVICE:
 if(prop->len >= (int)sizeof(int))
                ret = btif_config_get_int(bdstr, BTIF_STORAGE_PATH_REMOTE_DEVTYPE, (int*)prop->val);
 break;
 case BT_PROPERTY_UUIDS:
 {
 char value[1280];
 int size = sizeof(value);
 if(btif_config_get_str(bdstr,
                                    BTIF_STORAGE_PATH_REMOTE_SERVICE, value, &size))
 {
 bt_uuid_t *p_uuid = (bt_uuid_t*)prop->val;
 uint32_t num_uuids = 0;
                btif_in_split_uuids_string_to_list(value, p_uuid, &num_uuids);
                prop->len = num_uuids * sizeof(bt_uuid_t);
                ret = TRUE;
 }
 else
 {
                prop->val = NULL;
                prop->len = 0;
 }
 } break;

 case BT_PROPERTY_REMOTE_VERSION_INFO:
 {
 bt_remote_version_t *info = (bt_remote_version_t *)prop->val;

 if(prop->len >= (int)sizeof(bt_remote_version_t))
 {
                ret = btif_config_get_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_VER_MFCT, &info->manufacturer);

 if (ret == TRUE)
                    ret = btif_config_get_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_VER_VER, &info->version);

 if (ret == TRUE)
                    ret = btif_config_get_int(bdstr,
                                BTIF_STORAGE_PATH_REMOTE_VER_SUBVER, &info->sub_ver);
 }
 } break;

 default:
            BTIF_TRACE_ERROR("Unknow prop type:%d", prop->type);
 return FALSE;
 }
 return ret;
}
