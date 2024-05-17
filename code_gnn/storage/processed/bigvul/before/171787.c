void btif_dm_update_ble_remote_properties( BD_ADDR bd_addr, BD_NAME bd_name,
                                           tBT_DEVICE_TYPE dev_type)
{
   btif_update_remote_properties(bd_addr,bd_name,NULL,dev_type);
}
