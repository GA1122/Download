bt_status_t btif_dm_ssp_reply(const bt_bdaddr_t *bd_addr,
 bt_ssp_variant_t variant, uint8_t accept,
 uint32_t passkey)
{
    UNUSED(passkey);

 if (variant == BT_SSP_VARIANT_PASSKEY_ENTRY)
 {
  
        BTIF_TRACE_WARNING("%s: Not implemented", __FUNCTION__);
 return BT_STATUS_FAIL;
 }
  
    BTIF_TRACE_EVENT("%s: accept=%d", __FUNCTION__, accept);
#if (defined(BLE_INCLUDED) && (BLE_INCLUDED == TRUE))
 if (pairing_cb.is_le_only)
 {
 if(pairing_cb.is_le_nc)
 {
            BTA_DmBleConfirmReply((UINT8 *)bd_addr->address,accept);
 } else {
 if (accept)
                BTA_DmBleSecurityGrant((UINT8 *)bd_addr->address,BTA_DM_SEC_GRANTED);
 else
                BTA_DmBleSecurityGrant((UINT8 *)bd_addr->address,BTA_DM_SEC_PAIR_NOT_SPT);
 }
 } else {
        BTA_DmConfirm( (UINT8 *)bd_addr->address, accept);
 }
#else
    BTA_DmConfirm( (UINT8 *)bd_addr->address, accept);
#endif
 return BT_STATUS_SUCCESS;
}
