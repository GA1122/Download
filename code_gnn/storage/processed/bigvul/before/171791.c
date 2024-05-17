static void btif_update_remote_properties(BD_ADDR bd_addr, BD_NAME bd_name,
                                          DEV_CLASS dev_class, tBT_DEVICE_TYPE device_type)
{
 int num_properties = 0;
 bt_property_t properties[3];
 bt_bdaddr_t bdaddr;
 bt_status_t status;
    UINT32 cod;
 bt_device_type_t dev_type;

    memset(properties, 0, sizeof(properties));
    bdcpy(bdaddr.address, bd_addr);

  
 if (strlen((const char *) bd_name))
 {
        BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                            BT_PROPERTY_BDNAME, strlen((char *)bd_name), bd_name);
        status = btif_storage_set_remote_device_property(&bdaddr, &properties[num_properties]);
        ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device name", status);
        num_properties++;
 }

  
    cod = devclass2uint(dev_class);
    BTIF_TRACE_DEBUG("%s cod is 0x%06x", __func__, cod);
 if ( cod == 0) {
  
        BTIF_TRACE_DEBUG("%s cod is 0, checking cod from storage", __func__);
        BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
            BT_PROPERTY_CLASS_OF_DEVICE, sizeof(cod), &cod);
        status = btif_storage_get_remote_device_property(&bdaddr, &properties[num_properties]);
        BTIF_TRACE_DEBUG("%s cod retrieved from storage is 0x%06x", __func__, cod);
 if ( cod == 0) {
            BTIF_TRACE_DEBUG("%s cod is again 0, set as unclassified", __func__);
            cod = COD_UNCLASSIFIED;
 }
 }

    BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                        BT_PROPERTY_CLASS_OF_DEVICE, sizeof(cod), &cod);
    status = btif_storage_set_remote_device_property(&bdaddr, &properties[num_properties]);
    ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device class", status);
    num_properties++;

  
 bt_property_t prop_name;
 uint8_t remote_dev_type;
    BTIF_STORAGE_FILL_PROPERTY(&prop_name, BT_PROPERTY_TYPE_OF_DEVICE,
 sizeof(uint8_t), &remote_dev_type);
 if (btif_storage_get_remote_device_property(&bdaddr, &prop_name) == BT_STATUS_SUCCESS)
         dev_type = remote_dev_type | device_type;
 else
         dev_type = device_type;

    BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                        BT_PROPERTY_TYPE_OF_DEVICE, sizeof(dev_type), &dev_type);
    status = btif_storage_set_remote_device_property(&bdaddr, &properties[num_properties]);
    ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device type", status);
    num_properties++;

    HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                     status, &bdaddr, num_properties, properties);
}
