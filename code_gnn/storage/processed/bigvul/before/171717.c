static bt_status_t btif_in_get_remote_device_properties(bt_bdaddr_t *bd_addr)
{
 bt_property_t remote_properties[8];
 uint32_t num_props = 0;

 bt_bdname_t name, alias;
 uint32_t cod, devtype;
 bt_uuid_t remote_uuids[BT_MAX_NUM_UUIDS];

    memset(remote_properties, 0, sizeof(remote_properties));
    BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props], BT_PROPERTY_BDNAME,
 sizeof(name), &name);
    btif_storage_get_remote_device_property(bd_addr,
 &remote_properties[num_props]);
    num_props++;

    BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props], BT_PROPERTY_REMOTE_FRIENDLY_NAME,
 sizeof(alias), &alias);
    btif_storage_get_remote_device_property(bd_addr,
 &remote_properties[num_props]);
    num_props++;

    BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props], BT_PROPERTY_CLASS_OF_DEVICE,
 sizeof(cod), &cod);
    btif_storage_get_remote_device_property(bd_addr,
 &remote_properties[num_props]);
    num_props++;

    BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props], BT_PROPERTY_TYPE_OF_DEVICE,
 sizeof(devtype), &devtype);
    btif_storage_get_remote_device_property(bd_addr,
 &remote_properties[num_props]);
    num_props++;

    BTIF_STORAGE_FILL_PROPERTY(&remote_properties[num_props], BT_PROPERTY_UUIDS,
 sizeof(remote_uuids), remote_uuids);
    btif_storage_get_remote_device_property(bd_addr,
 &remote_properties[num_props]);
    num_props++;

    HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                     BT_STATUS_SUCCESS, bd_addr, num_props, remote_properties);

 return BT_STATUS_SUCCESS;
}
