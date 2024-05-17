bt_status_t btif_storage_add_remote_device(bt_bdaddr_t *remote_bd_addr,
 uint32_t num_properties,
 bt_property_t *properties)
{
 uint32_t i = 0;
  
 for (i=0; i < num_properties; i++)
 {
  
 if (properties[i].type == BT_PROPERTY_REMOTE_RSSI)
 continue;

  
 if (properties[i].type == BT_PROPERTY_BDADDR)
 {
 bt_property_t addr_prop;
            memcpy(&addr_prop, &properties[i], sizeof(bt_property_t));
            addr_prop.type = BT_PROPERTY_REMOTE_DEVICE_TIMESTAMP;
            btif_storage_set_remote_device_property(remote_bd_addr,
 &addr_prop);
 }
 else
 {
            btif_storage_set_remote_device_property(remote_bd_addr,
 &properties[i]);
 }
 }
 return BT_STATUS_SUCCESS;
}
