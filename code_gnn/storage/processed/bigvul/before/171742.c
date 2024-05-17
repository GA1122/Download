static void btif_dm_auth_cmpl_evt (tBTA_DM_AUTH_CMPL *p_auth_cmpl)
{
  
 bt_bdaddr_t bd_addr;
 bt_status_t status = BT_STATUS_FAIL;
 bt_bond_state_t state = BT_BOND_STATE_NONE;
    BOOLEAN skip_sdp = FALSE;

    BTIF_TRACE_DEBUG("%s: bond state=%d", __func__, pairing_cb.state);

    bdcpy(bd_addr.address, p_auth_cmpl->bd_addr);
 if ( (p_auth_cmpl->success == TRUE) && (p_auth_cmpl->key_present) )
 {
 if ((p_auth_cmpl->key_type < HCI_LKEY_TYPE_DEBUG_COMB) ||
 (p_auth_cmpl->key_type == HCI_LKEY_TYPE_AUTH_COMB) ||
 (p_auth_cmpl->key_type == HCI_LKEY_TYPE_CHANGED_COMB) ||
 (p_auth_cmpl->key_type == HCI_LKEY_TYPE_AUTH_COMB_P_256) ||
            pairing_cb.bond_type == BOND_TYPE_PERSISTENT)
 {
 bt_status_t ret;
            BTIF_TRACE_DEBUG("%s: Storing link key. key_type=0x%x, bond_type=%d",
                __FUNCTION__, p_auth_cmpl->key_type, pairing_cb.bond_type);
            ret = btif_storage_add_bonded_device(&bd_addr,
                                p_auth_cmpl->key, p_auth_cmpl->key_type,
                                pairing_cb.pin_code_len);
            ASSERTC(ret == BT_STATUS_SUCCESS, "storing link key failed", ret);
 }
 else
 {
            BTIF_TRACE_DEBUG("%s: Temporary key. Not storing. key_type=0x%x, bond_type=%d",
                __FUNCTION__, p_auth_cmpl->key_type, pairing_cb.bond_type);
 if(pairing_cb.bond_type == BOND_TYPE_TEMPORARY)
 {
                BTIF_TRACE_DEBUG("%s: sending BT_BOND_STATE_NONE for Temp pairing",
                        __FUNCTION__);
                btif_storage_remove_bonded_device(&bd_addr);
                bond_state_changed(BT_STATUS_SUCCESS, &bd_addr, BT_BOND_STATE_NONE);
 return;
 }
 }
 }

 if (p_auth_cmpl->success)
 {
#if BLE_INCLUDED == TRUE
        btif_storage_set_remote_addr_type(&bd_addr, p_auth_cmpl->addr_type);
#endif
        btif_update_remote_properties(p_auth_cmpl->bd_addr,
                                      p_auth_cmpl->bd_name, NULL, p_auth_cmpl->dev_type);
        pairing_cb.timeout_retries = 0;
        status = BT_STATUS_SUCCESS;
        state = BT_BOND_STATE_BONDED;
        bdcpy(bd_addr.address, p_auth_cmpl->bd_addr);

 if (check_sdp_bl(&bd_addr) && check_cod_hid(&bd_addr, COD_HID_MAJOR))
 {
            LOG_WARN("%s:skip SDP", __FUNCTION__);
            skip_sdp = TRUE;
 }
 if(!pairing_cb.is_local_initiated && skip_sdp)
 {
            bond_state_changed(status, &bd_addr, state);

            LOG_WARN("%s: Incoming HID Connection",__FUNCTION__);
 bt_property_t prop;
 bt_bdaddr_t bd_addr;
 bt_uuid_t  uuid;
 char uuid_str[128] = UUID_HUMAN_INTERFACE_DEVICE;

            string_to_uuid(uuid_str, &uuid);

            prop.type = BT_PROPERTY_UUIDS;
            prop.val = uuid.uu;
            prop.len = MAX_UUID_SIZE;

  
            HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                             BT_STATUS_SUCCESS, &bd_addr, 1, &prop);
 }
 else
 {
  
            pairing_cb.sdp_attempts = 1;;

#if BLE_INCLUDED == TRUE
  
 if(pairing_cb.state == BT_BOND_STATE_BONDING &&
 (bdcmp(p_auth_cmpl->bd_addr, pairing_cb.bd_addr) != 0))
 {
                BTIF_TRACE_DEBUG("%s: bonding initiated due to cross key, adding static address",
                                 __func__);
                bdcpy(pairing_cb.static_bdaddr.address, p_auth_cmpl->bd_addr);
 }
#endif

 if(btif_dm_inquiry_in_progress)
                btif_dm_cancel_discovery();

            btif_dm_get_remote_services(&bd_addr);
 }
 }
 else
 {
 switch(p_auth_cmpl->fail_reason)
 {
 case HCI_ERR_PAGE_TIMEOUT:
 if (interop_match(INTEROP_AUTO_RETRY_PAIRING, &bd_addr)
 && pairing_cb.timeout_retries)
 {
                    BTIF_TRACE_WARNING("%s() - Pairing timeout; retrying (%d) ...", __FUNCTION__, pairing_cb.timeout_retries);
 --pairing_cb.timeout_retries;
                    btif_dm_cb_create_bond (&bd_addr, BTA_TRANSPORT_UNKNOWN);
 return;
 }
  
 case HCI_ERR_CONNECTION_TOUT:
                status =  BT_STATUS_RMT_DEV_DOWN;
 break;

 case HCI_ERR_PAIRING_NOT_ALLOWED:
                status = BT_STATUS_AUTH_REJECTED;
 break;

 case HCI_ERR_LMP_RESPONSE_TIMEOUT:
                status =  BT_STATUS_AUTH_FAILURE;
 break;

  
 case HCI_ERR_AUTH_FAILURE:
 case HCI_ERR_KEY_MISSING:
                btif_storage_remove_bonded_device(&bd_addr);
 case HCI_ERR_HOST_REJECT_SECURITY:
 case HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE:
 case HCI_ERR_UNIT_KEY_USED:
 case HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED:
 case HCI_ERR_INSUFFCIENT_SECURITY:
 case HCI_ERR_PEER_USER:
 case HCI_ERR_UNSPECIFIED:
                BTIF_TRACE_DEBUG(" %s() Authentication fail reason %d",
                    __FUNCTION__, p_auth_cmpl->fail_reason);
 if (pairing_cb.autopair_attempts  == 1)
 {
                    BTIF_TRACE_DEBUG("%s(): Adding device to blacklist ", __FUNCTION__);

  
 if (check_cod(&bd_addr, COD_AV_HEADSETS) ||
                        check_cod(&bd_addr, COD_AV_HANDSFREE) ||
                        check_cod(&bd_addr, COD_AV_HEADPHONES) ||
                        check_cod(&bd_addr, COD_AV_PORTABLE_AUDIO) ||
                        check_cod(&bd_addr, COD_AV_HIFI_AUDIO) ||
                        check_cod(&bd_addr, COD_HID_POINTING))
 {
                        btif_storage_add_device_to_autopair_blacklist (&bd_addr);
 }
                    pairing_cb.autopair_attempts++;

  
                    BTIF_TRACE_DEBUG("%s() auto pair failed. Reinitiate Bond", __FUNCTION__);
                    btif_dm_cb_create_bond (&bd_addr, BTA_TRANSPORT_UNKNOWN);
 return;
 }
 else
 {
  
                    status =  BT_STATUS_AUTH_FAILURE;
 }
 break;

 default:
                status =  BT_STATUS_FAIL;
 }
  
 if (check_cod(&bd_addr, COD_HID_POINTING)) {
  
            BTIF_TRACE_DEBUG("%s(): removing hid pointing device from nvram", __FUNCTION__);
            btif_storage_remove_bonded_device(&bd_addr);
 }
        bond_state_changed(status, &bd_addr, state);
 }
}
