static bt_status_t btif_in_fetch_bonded_device(const char *bdstr)
{
    BOOLEAN bt_linkkey_file_found=FALSE;

        LINK_KEY link_key;
 size_t size = sizeof(link_key);
 if(btif_config_get_bin(bdstr, "LinkKey", (uint8_t *)link_key, &size))
 {
 int linkkey_type;
 if(btif_config_get_int(bdstr, "LinkKeyType", &linkkey_type))
 {
                bt_linkkey_file_found = TRUE;
 }
 else
 {
                bt_linkkey_file_found = FALSE;
 }
 }
#if (BLE_INCLUDED == TRUE)
 if((btif_in_fetch_bonded_ble_device(bdstr, FALSE, NULL) != BT_STATUS_SUCCESS)
 && (!bt_linkkey_file_found))
 {
            BTIF_TRACE_DEBUG("Remote device:%s, no link key or ble key found", bdstr);
 return BT_STATUS_FAIL;
 }
#else
 if((!bt_linkkey_file_found))
 {
            BTIF_TRACE_DEBUG("Remote device:%s, no link key found", bdstr);
 return BT_STATUS_FAIL;
 }
#endif
 return BT_STATUS_SUCCESS;
}
