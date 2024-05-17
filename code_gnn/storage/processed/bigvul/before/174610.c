void btm_sec_encrypt_change (UINT16 handle, UINT8 status, UINT8 encr_enable)
{
    tBTM_SEC_DEV_REC  *p_dev_rec = btm_find_dev_by_handle (handle);
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
    tACL_CONN       *p_acl = NULL;
    UINT8           acl_idx = btm_handle_to_acl_index(handle);
#endif
    BTM_TRACE_EVENT ("Security Manager: encrypt_change status:%d State:%d, encr_enable = %d",
                      status, (p_dev_rec) ? p_dev_rec->sec_state : 0, encr_enable);
    BTM_TRACE_DEBUG ("before update p_dev_rec->sec_flags=0x%x", (p_dev_rec) ? p_dev_rec->sec_flags : 0 );

  
  
 if ((status == HCI_ERR_LMP_ERR_TRANS_COLLISION) || (status == HCI_ERR_DIFF_TRANSACTION_COLLISION))
 {
        btm_sec_auth_collision(handle);
 return;
 }
    btm_cb.collision_start_time = 0;

 if (!p_dev_rec)
 return;

 if ((status == HCI_SUCCESS) && encr_enable)
 {
 if (p_dev_rec->hci_handle == handle)
            p_dev_rec->sec_flags |= (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED);
 else
            p_dev_rec->sec_flags |= (BTM_SEC_LE_AUTHENTICATED | BTM_SEC_LE_ENCRYPTED);
 }

  
  
 if ((status == HCI_SUCCESS) && !encr_enable)
 {
 if (p_dev_rec->hci_handle == handle)
            p_dev_rec->sec_flags &= ~BTM_SEC_ENCRYPTED;
 else
            p_dev_rec->sec_flags &= ~BTM_SEC_LE_ENCRYPTED;
 }

    BTM_TRACE_DEBUG ("after update p_dev_rec->sec_flags=0x%x", p_dev_rec->sec_flags );

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
 if (acl_idx != MAX_L2CAP_LINKS )
        p_acl = &btm_cb.acl_db[acl_idx];

 if (p_acl && p_acl->transport == BT_TRANSPORT_LE)
 {
 if (status == HCI_ERR_KEY_MISSING || status == HCI_ERR_AUTH_FAILURE
 ||status == HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE)
            p_dev_rec->sec_flags &= ~ (BTM_SEC_LE_LINK_KEY_KNOWN);
        btm_ble_link_encrypted(p_dev_rec->bd_addr, encr_enable);
 return;
 }
 else
  
        p_dev_rec->enc_key_size = 16;
#endif

  
 if (p_dev_rec->sec_state != BTM_SEC_STATE_ENCRYPTING)
 {
 if (BTM_SEC_STATE_DELAY_FOR_ENC == p_dev_rec->sec_state)
 {
            p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
            p_dev_rec->p_callback = NULL;
            l2cu_resubmit_pending_sec_req (p_dev_rec->bd_addr);
 }
 return;
 }

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

  
 if (status != HCI_SUCCESS)
 {
        btm_sec_dev_rec_cback_event (p_dev_rec, BTM_ERR_PROCESSING, FALSE);
 return;
 }

  
    status = (UINT8)btm_sec_execute_procedure (p_dev_rec);

  
 if (status != BTM_CMD_STARTED)
        btm_sec_dev_rec_cback_event (p_dev_rec, status, FALSE);
}
