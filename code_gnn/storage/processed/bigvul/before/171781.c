static void btif_dm_search_services_evt(UINT16 event, char *p_param)
{
    tBTA_DM_SEARCH *p_data = (tBTA_DM_SEARCH*)p_param;

    BTIF_TRACE_EVENT("%s:  event = %d", __FUNCTION__, event);
 switch (event)
 {
 case BTA_DM_DISC_RES_EVT:
 {
 bt_property_t prop;
 uint32_t i = 0;
 bt_bdaddr_t bd_addr;
 bt_status_t ret;

            bdcpy(bd_addr.address, p_data->disc_res.bd_addr);

            BTIF_TRACE_DEBUG("%s:(result=0x%x, services 0x%x)", __FUNCTION__,
                    p_data->disc_res.result, p_data->disc_res.services);
 if ((p_data->disc_res.result != BTA_SUCCESS) &&
 (pairing_cb.state == BT_BOND_STATE_BONDING ) &&
 (pairing_cb.sdp_attempts < BTIF_DM_MAX_SDP_ATTEMPTS_AFTER_PAIRING))
 {
                BTIF_TRACE_WARNING("%s:SDP failed after bonding re-attempting", __FUNCTION__);
                pairing_cb.sdp_attempts++;
                btif_dm_get_remote_services(&bd_addr);
 return;
 }
            prop.type = BT_PROPERTY_UUIDS;
            prop.len = 0;
 if ((p_data->disc_res.result == BTA_SUCCESS) && (p_data->disc_res.num_uuids > 0))
 {
                 prop.val = p_data->disc_res.p_uuid_list;
                 prop.len = p_data->disc_res.num_uuids * MAX_UUID_SIZE;
 for (i=0; i < p_data->disc_res.num_uuids; i++)
 {
 char temp[256];
                      uuid_to_string_legacy((bt_uuid_t*)(p_data->disc_res.p_uuid_list + (i*MAX_UUID_SIZE)), temp);
                      LOG_INFO("%s index:%d uuid:%s", __func__, i, temp);
 }
 }

  
 if ((pairing_cb.state == BT_BOND_STATE_BONDING) &&
 ((bdcmp(p_data->disc_res.bd_addr, pairing_cb.bd_addr) == 0) ||
 (bdcmp(p_data->disc_res.bd_addr, pairing_cb.static_bdaddr.address) == 0)) &&
                  pairing_cb.sdp_attempts > 0)
 {
                 BTIF_TRACE_DEBUG("%s Remote Service SDP done. Call bond_state_changed_cb BONDED",
                                   __FUNCTION__);
                 pairing_cb.sdp_attempts  = 0;

 if (bdcmp(p_data->disc_res.bd_addr, pairing_cb.static_bdaddr.address) == 0)
                    bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDING);

                 bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_BONDED);
 }

 if (p_data->disc_res.num_uuids != 0)
 {
  
                ret = btif_storage_set_remote_device_property(&bd_addr, &prop);
                ASSERTC(ret == BT_STATUS_SUCCESS, "storing remote services failed", ret);
  
                HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                                 BT_STATUS_SUCCESS, &bd_addr, 1, &prop);
 }
 }
 break;

 case BTA_DM_DISC_CMPL_EVT:
  
 break;

#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
 case BTA_DM_DISC_BLE_RES_EVT:
             BTIF_TRACE_DEBUG("%s:, services 0x%x)", __FUNCTION__,
                                p_data->disc_ble_res.service.uu.uuid16);
 bt_uuid_t  uuid;
 int i = 0;
 int j = 15;
 if (p_data->disc_ble_res.service.uu.uuid16 == UUID_SERVCLASS_LE_HID)
 {
                BTIF_TRACE_DEBUG("%s: Found HOGP UUID",__FUNCTION__);
 bt_property_t prop;
 bt_bdaddr_t bd_addr;
 char temp[256];
 bt_status_t ret;

                bta_gatt_convert_uuid16_to_uuid128(uuid.uu,p_data->disc_ble_res.service.uu.uuid16);

 while(i < j )
 {
 unsigned char c = uuid.uu[j];
                    uuid.uu[j] = uuid.uu[i];
                    uuid.uu[i] = c;
                    i++;
                    j--;
 }

                uuid_to_string_legacy(&uuid, temp);
                LOG_INFO("%s uuid:%s", __func__, temp);

                bdcpy(bd_addr.address, p_data->disc_ble_res.bd_addr);
                prop.type = BT_PROPERTY_UUIDS;
                prop.val = uuid.uu;
                prop.len = MAX_UUID_SIZE;

  
                ret = btif_storage_set_remote_device_property(&bd_addr, &prop);
                ASSERTC(ret == BT_STATUS_SUCCESS, "storing remote services failed", ret);

  
                HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                                 BT_STATUS_SUCCESS, &bd_addr, 1, &prop);

 }
 break;
#endif  

 default:
 {
            ASSERTC(0, "unhandled search services event", event);
 }
 break;
 }
}
