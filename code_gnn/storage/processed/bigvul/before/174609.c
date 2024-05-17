void btm_sec_disconnected (UINT16 handle, UINT8 reason)
{
    tBTM_SEC_DEV_REC  *p_dev_rec = btm_find_dev_by_handle (handle);
    UINT8             old_pairing_flags = btm_cb.pairing_flags;
 int               result = HCI_ERR_AUTH_FAILURE;
    tBTM_SEC_CALLBACK   *p_callback = NULL;
    tBT_TRANSPORT      transport = BT_TRANSPORT_BR_EDR;

  
    btm_cb.discing = FALSE;

    btm_acl_resubmit_page();

 if (!p_dev_rec)
 return;

    transport  = (handle == p_dev_rec->hci_handle) ? BT_TRANSPORT_BR_EDR: BT_TRANSPORT_LE;

    p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING;  

#if BTM_DISC_DURING_RS == TRUE
    BTM_TRACE_ERROR("btm_sec_disconnected - Clearing Pending flag");
    p_dev_rec->rs_disc_pending = BTM_SEC_RS_NOT_PENDING;  
#endif

  
    p_dev_rec->sm4 &= BTM_SM4_TRUE;

    BTM_TRACE_EVENT("btm_sec_disconnected() sec_req:x%x  State: %s   reason:%d bda:%04x%08x RName:%s",
                     p_dev_rec->security_required, btm_pair_state_descr(btm_cb.pairing_state), reason, (p_dev_rec->bd_addr[0]<<8)+p_dev_rec->bd_addr[1],
 (p_dev_rec->bd_addr[2]<<24)+(p_dev_rec->bd_addr[3]<<16)+(p_dev_rec->bd_addr[4]<<8)+p_dev_rec->bd_addr[5], p_dev_rec->sec_bd_name);

    BTM_TRACE_EVENT("before Update sec_flags=0x%x", p_dev_rec->sec_flags);

  
 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (memcmp (btm_cb.pairing_bda, p_dev_rec->bd_addr, BD_ADDR_LEN) == 0))
 {
        btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
        p_dev_rec->sec_flags &= ~BTM_SEC_LINK_KEY_KNOWN;
 if (btm_cb.api.p_auth_complete_callback)
 {
  
 if (reason == HCI_ERR_REPEATED_ATTEMPTS)
 {
                result = HCI_ERR_REPEATED_ATTEMPTS;
 }
 else if (old_pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
 {
                result = HCI_ERR_HOST_REJECT_SECURITY;
 }
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,     p_dev_rec->dev_class,
                                                    p_dev_rec->sec_bd_name, result);
 }
 }

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
    p_dev_rec->enc_key_size = 0;
    btm_ble_update_mode_operation(HCI_ROLE_UNKNOWN, p_dev_rec->bd_addr, HCI_SUCCESS);
  

 if (transport == BT_TRANSPORT_LE)
 {
        p_dev_rec->ble_hci_handle = BTM_SEC_INVALID_HANDLE;
        p_dev_rec->sec_flags &= ~(BTM_SEC_LE_AUTHENTICATED|BTM_SEC_LE_ENCRYPTED);
 }
 else
#endif
 {
        p_dev_rec->hci_handle = BTM_SEC_INVALID_HANDLE;
        p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHORIZED | BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED | BTM_SEC_ROLE_SWITCHED);
 }

    p_dev_rec->sec_state  = BTM_SEC_STATE_IDLE;
    p_dev_rec->security_required = BTM_SEC_NONE;

    p_callback = p_dev_rec->p_callback;

  
 if(p_callback)
 {
        p_dev_rec->p_callback = NULL;  
 (*p_callback) (p_dev_rec->bd_addr, transport, p_dev_rec->p_ref_data, BTM_ERR_PROCESSING);
 }

    BTM_TRACE_EVENT("after Update sec_flags=0x%x", p_dev_rec->sec_flags);
}
