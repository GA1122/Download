bt_status_t btif_dm_cancel_bond(const bt_bdaddr_t *bd_addr)
{
 bdstr_t bdstr;

    BTIF_TRACE_EVENT("%s: bd_addr=%s", __FUNCTION__, bdaddr_to_string(bd_addr, bdstr, sizeof(bdstr)));

  
 if (pairing_cb.state == BT_BOND_STATE_BONDING)
 {

#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))

 if (pairing_cb.is_ssp)
 {
 if (pairing_cb.is_le_only)
 {
                BTA_DmBleSecurityGrant((UINT8 *)bd_addr->address,BTA_DM_SEC_PAIR_NOT_SPT);
 }
 else
 {
                BTA_DmConfirm( (UINT8 *)bd_addr->address, FALSE);
                BTA_DmBondCancel ((UINT8 *)bd_addr->address);
                btif_storage_remove_bonded_device((bt_bdaddr_t *)bd_addr);
 }
 }
 else
 {
 if (pairing_cb.is_le_only)
 {
                BTA_DmBondCancel ((UINT8 *)bd_addr->address);
 }
 else
 {
                BTA_DmPinReply( (UINT8 *)bd_addr->address, FALSE, 0, NULL);
 }
  
        BTA_DmBondCancel ((UINT8 *)bd_addr->address);
 }

#else
 if (pairing_cb.is_ssp)
 {
            BTA_DmConfirm( (UINT8 *)bd_addr->address, FALSE);
 }
 else
 {
            BTA_DmPinReply( (UINT8 *)bd_addr->address, FALSE, 0, NULL);
 }
  
        BTA_DmBondCancel ((UINT8 *)bd_addr->address);
        btif_storage_remove_bonded_device((bt_bdaddr_t *)bd_addr);
#endif
 }

 return BT_STATUS_SUCCESS;
}
