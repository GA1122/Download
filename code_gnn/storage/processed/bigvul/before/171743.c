static void btif_dm_ble_auth_cmpl_evt (tBTA_DM_AUTH_CMPL *p_auth_cmpl)
{
  
 bt_bdaddr_t bd_addr;
 bt_status_t status = BT_STATUS_FAIL;
 bt_bond_state_t state = BT_BOND_STATE_NONE;

    bdcpy(bd_addr.address, p_auth_cmpl->bd_addr);
 if ( (p_auth_cmpl->success == TRUE) && (p_auth_cmpl->key_present) )
 {
  
 }
 if (p_auth_cmpl->success)
 {
        status = BT_STATUS_SUCCESS;
        state = BT_BOND_STATE_BONDED;
 int addr_type;
 bt_bdaddr_t bdaddr;
        bdcpy(bdaddr.address, p_auth_cmpl->bd_addr);
 if (btif_storage_get_remote_addr_type(&bdaddr, &addr_type) != BT_STATUS_SUCCESS)
            btif_storage_set_remote_addr_type(&bdaddr, p_auth_cmpl->addr_type);

  
 if (btm_get_bond_type_dev(p_auth_cmpl->bd_addr) == BOND_TYPE_TEMPORARY) {
            BTIF_TRACE_DEBUG("%s: sending BT_BOND_STATE_NONE for Temp pairing",
                             __func__);
            btif_storage_remove_bonded_device(&bdaddr);
            state = BT_BOND_STATE_NONE;
 } else {
            btif_dm_save_ble_bonding_keys();
            BTA_GATTC_Refresh(bd_addr.address);
            btif_dm_get_remote_services_by_transport(&bd_addr, BTA_GATT_TRANSPORT_LE);
 }
 }
 else
 {
  
 switch (p_auth_cmpl->fail_reason)
 {
 case BTA_DM_AUTH_SMP_PAIR_AUTH_FAIL:
 case BTA_DM_AUTH_SMP_CONFIRM_VALUE_FAIL:
                btif_dm_remove_ble_bonding_keys();
                status = BT_STATUS_AUTH_FAILURE;
 break;
 case BTA_DM_AUTH_SMP_PAIR_NOT_SUPPORT:
                status = BT_STATUS_AUTH_REJECTED;
 break;
 default:
                btif_dm_remove_ble_bonding_keys();
                status =  BT_STATUS_FAIL;
 break;
 }
 }
    bond_state_changed(status, &bd_addr, state);
}
