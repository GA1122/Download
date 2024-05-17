BOOLEAN check_cod_hid(const bt_bdaddr_t *remote_bdaddr, uint32_t cod)
{
 uint32_t    remote_cod;
 bt_property_t prop_name;

  
    BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_CLASS_OF_DEVICE,
 sizeof(uint32_t), &remote_cod);
 if (btif_storage_get_remote_device_property((bt_bdaddr_t *)remote_bdaddr,
 &prop_name) == BT_STATUS_SUCCESS)
 {
        BTIF_TRACE_DEBUG("%s: remote_cod = 0x%06x", __FUNCTION__, remote_cod);
 if ((remote_cod & 0x700) == cod)
 return TRUE;
 }
 return FALSE;
}
