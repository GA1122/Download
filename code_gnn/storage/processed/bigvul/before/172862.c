bt_status_t btif_storage_get_remote_device_property(bt_bdaddr_t *remote_bd_addr,
 bt_property_t *property)
{
 return cfg2prop(remote_bd_addr, property) ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}
