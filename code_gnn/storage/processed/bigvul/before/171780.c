static void btif_dm_search_devices_evt (UINT16 event, char *p_param)
{
    tBTA_DM_SEARCH *p_search_data;
    BTIF_TRACE_EVENT("%s event=%s", __FUNCTION__, dump_dm_search_event(event));

 switch (event)
 {
 case BTA_DM_DISC_RES_EVT:
 {
            p_search_data = (tBTA_DM_SEARCH *)p_param;
  
 if (strlen((const char *) p_search_data->disc_res.bd_name))
 {
 bt_property_t properties[1];
 bt_bdaddr_t bdaddr;
 bt_status_t status;

                properties[0].type = BT_PROPERTY_BDNAME;
                properties[0].val = p_search_data->disc_res.bd_name;
                properties[0].len = strlen((char *)p_search_data->disc_res.bd_name);
                bdcpy(bdaddr.address, p_search_data->disc_res.bd_addr);

                status = btif_storage_set_remote_device_property(&bdaddr, &properties[0]);
                ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device property", status);
                HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                                 status, &bdaddr, 1, properties);
 }
  
 }
 break;

 case BTA_DM_INQ_RES_EVT:
 {
  
            UINT32 cod;
 bt_bdname_t bdname;
 bt_bdaddr_t bdaddr;
            UINT8 remote_name_len;
            tBTA_SERVICE_MASK services = 0;
 bdstr_t bdstr;

            p_search_data = (tBTA_DM_SEARCH *)p_param;
            bdcpy(bdaddr.address, p_search_data->inq_res.bd_addr);

            BTIF_TRACE_DEBUG("%s() %s device_type = 0x%x\n", __FUNCTION__, bdaddr_to_string(&bdaddr, bdstr, sizeof(bdstr)),
#if (BLE_INCLUDED == TRUE)
                    p_search_data->inq_res.device_type);
#else
                    BT_DEVICE_TYPE_BREDR);
#endif
            bdname.name[0] = 0;

            cod = devclass2uint (p_search_data->inq_res.dev_class);

 if (cod == 0) {
                LOG_DEBUG("%s cod is 0, set as unclassified", __func__);
                cod = COD_UNCLASSIFIED;
 }

 if (!check_eir_remote_name(p_search_data, bdname.name, &remote_name_len))
                check_cached_remote_name(p_search_data, bdname.name, &remote_name_len);

  
 if (p_search_data->inq_res.p_eir)
 {
                BTA_GetEirService(p_search_data->inq_res.p_eir, &services);
                BTIF_TRACE_DEBUG("%s()EIR BTA services = %08X", __FUNCTION__, (UINT32)services);
  
 }


 {
 bt_property_t properties[5];
 bt_device_type_t dev_type;
 uint32_t num_properties = 0;
 bt_status_t status;
 int addr_type = 0;

                memset(properties, 0, sizeof(properties));
  
                BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                    BT_PROPERTY_BDADDR, sizeof(bdaddr), &bdaddr);
                num_properties++;
  
  
 if (bdname.name[0])
 {
                    BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                               BT_PROPERTY_BDNAME,
                                               strlen((char *)bdname.name), &bdname);
                    num_properties++;
 }

  
                BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                    BT_PROPERTY_CLASS_OF_DEVICE, sizeof(cod), &cod);
                num_properties++;
  
#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
  

  
 int stored_device_type = 0;
 if (btif_get_device_type(bdaddr.address, &stored_device_type) &&
 ((stored_device_type == BT_DEVICE_TYPE_BLE &&
                        p_search_data->inq_res.device_type == BT_DEVICE_TYPE_BREDR) ||
 (stored_device_type == BT_DEVICE_TYPE_BREDR &&
                        p_search_data->inq_res.device_type == BT_DEVICE_TYPE_BLE))) {
                    dev_type = BT_DEVICE_TYPE_DUMO;
 } else {
                    dev_type = p_search_data->inq_res.device_type;
 }

 if (p_search_data->inq_res.device_type == BT_DEVICE_TYPE_BLE)
                    addr_type = p_search_data->inq_res.ble_addr_type;
#else
                dev_type = BT_DEVICE_TYPE_BREDR;
#endif
                BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                    BT_PROPERTY_TYPE_OF_DEVICE, sizeof(dev_type), &dev_type);
                num_properties++;
  
                BTIF_STORAGE_FILL_PROPERTY(&properties[num_properties],
                                    BT_PROPERTY_REMOTE_RSSI, sizeof(int8_t),
 &(p_search_data->inq_res.rssi));
                num_properties++;

                status = btif_storage_add_remote_device(&bdaddr, num_properties, properties);
                ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote device (inquiry)", status);
#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
                status = btif_storage_set_remote_addr_type(&bdaddr, addr_type);
                ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote addr type (inquiry)", status);
#endif
  
                HAL_CBACK(bt_hal_cbacks, device_found_cb,
                                 num_properties, properties);
 }
 }
 break;

 case BTA_DM_INQ_CMPL_EVT:
 {
#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
            tBTA_DM_BLE_PF_FILT_PARAMS adv_filt_param;
            memset(&adv_filt_param, 0, sizeof(tBTA_DM_BLE_PF_FILT_PARAMS));
            BTA_DmBleScanFilterSetup(BTA_DM_BLE_SCAN_COND_DELETE, 0, &adv_filt_param, NULL,
                                     bte_scan_filt_param_cfg_evt, 0);
#endif
 }
 break;
 case BTA_DM_DISC_CMPL_EVT:
 {
            HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb, BT_DISCOVERY_STOPPED);
 }
 break;
 case BTA_DM_SEARCH_CANCEL_CMPL_EVT:
 {
  
 if (btif_dm_inquiry_in_progress == FALSE)
 {
#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
               tBTA_DM_BLE_PF_FILT_PARAMS adv_filt_param;
               memset(&adv_filt_param, 0, sizeof(tBTA_DM_BLE_PF_FILT_PARAMS));
               BTA_DmBleScanFilterSetup(BTA_DM_BLE_SCAN_COND_DELETE, 0, &adv_filt_param, NULL,
                                        bte_scan_filt_param_cfg_evt, 0);
#endif
               HAL_CBACK(bt_hal_cbacks, discovery_state_changed_cb, BT_DISCOVERY_STOPPED);
 }
 }
 break;
 }
}
