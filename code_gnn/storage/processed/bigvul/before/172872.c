bt_status_t btif_storage_set_adapter_property(bt_property_t *property)
{
 return prop2cfg(NULL, property) ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}
