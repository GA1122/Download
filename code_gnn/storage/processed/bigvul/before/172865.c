bt_status_t btif_storage_load_bonded_devices(void)
{
 btif_bonded_devices_t bonded_devices;
 uint32_t i = 0;
 bt_property_t adapter_props[6];
 uint32_t num_props = 0;
 bt_property_t remote_properties[8];
 bt_bdaddr_t addr;
 bt_bdname_t name, alias;
 bt_scan_mode_t mode;
 uint32_t disc_timeout;
 bt_bdaddr_t *devices_list;
 bt_uuid_t local_uuids[BT_MAX_NUM_UUIDS];
 bt_uuid_t remote_uuids[BT_MAX_NUM_UUIDS];
 uint32_t cod, devtype;

    btif_in_fetch_bonded_devices(&bonded_devices, 1);

  
 {
        memset(adapter_props, 0, sizeof(adapter_props));

  
        BTIF_STORAGE_GET_ADAPTER_PROP(BT_PROPERTY_BDADDR, &addr, sizeof(addr),
                                      adapter_props[num_props]);
        num_props++;

  
        BTIF_STORAGE_GET_ADAPTER_PROP(BT_PROPERTY_BDNAME, &name, sizeof(name),
                                      adapter_props[num_props]);
        num_props++;

  
  
        mode = BT_SCAN_MODE_NONE;
        adapter_props[num_props].type = BT_PROPERTY_ADAPTER_SCAN_MODE;
        adapter_props[num_props].len = sizeof(mode);
        adapter_props[num_props].val = &mode;
        num_props++;

  
        BTIF_STORAGE_GET_ADAPTER_PROP(BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT,
 &disc_timeout, sizeof(disc_timeout),
                                      adapter_props[num_props]);
        num_props++;

  
        devices_list = (bt_bdaddr_t*)osi_malloc(sizeof(bt_bdaddr_t)*bonded_devices.num_devices);
        adapter_props[num_props].type = BT_PROPERTY_ADAPTER_BONDED_DEVICES;
        adapter_props[num_props].len = bonded_devices.num_devices * sizeof(bt_bdaddr_t);
        adapter_props[num_props].val = devices_list;
 for (i=0; i < bonded_devices.num_devices; i++)
 {
            memcpy(devices_list + i, &bonded_devices.devices[i], sizeof(bt_bdaddr_t));
 }
        num_props++;

  
        BTIF_STORAGE_GET_ADAPTER_PROP(BT_PROPERTY_UUIDS,
                                      local_uuids, sizeof(local_uuids),
                                      adapter_props[num_props]);
        num_props++;

        btif_adapter_properties_evt(BT_STATUS_SUCCESS, num_props, adapter_props);

        osi_free(devices_list);
 }

    BTIF_TRACE_EVENT("%s: %d bonded devices found", __FUNCTION__, bonded_devices.num_devices);

 {
 for (i = 0; i < bonded_devices.num_devices; i++)
 {
 bt_bdaddr_t *p_remote_addr;

            num_props = 0;
            p_remote_addr = &bonded_devices.devices[i];
            memset(remote_properties, 0, sizeof(remote_properties));
            BTIF_STORAGE_GET_REMOTE_PROP(p_remote_addr, BT_PROPERTY_BDNAME,
 &name, sizeof(name),
                                         remote_properties[num_props]);
            num_props++;

            BTIF_STORAGE_GET_REMOTE_PROP(p_remote_addr, BT_PROPERTY_REMOTE_FRIENDLY_NAME,
 &alias, sizeof(alias),
                                         remote_properties[num_props]);
            num_props++;

            BTIF_STORAGE_GET_REMOTE_PROP(p_remote_addr, BT_PROPERTY_CLASS_OF_DEVICE,
 &cod, sizeof(cod),
                                         remote_properties[num_props]);
            num_props++;

            BTIF_STORAGE_GET_REMOTE_PROP(p_remote_addr, BT_PROPERTY_TYPE_OF_DEVICE,
 &devtype, sizeof(devtype),
                                         remote_properties[num_props]);
            num_props++;

            BTIF_STORAGE_GET_REMOTE_PROP(p_remote_addr, BT_PROPERTY_UUIDS,
                                         remote_uuids, sizeof(remote_uuids),
                                         remote_properties[num_props]);
            num_props++;

            btif_remote_properties_evt(BT_STATUS_SUCCESS, p_remote_addr,
                                       num_props, remote_properties);
 }
 }
 return BT_STATUS_SUCCESS;
}
