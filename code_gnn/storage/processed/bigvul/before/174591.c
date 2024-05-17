void btm_sec_auth_complete (UINT16 handle, UINT8 status)
{
    UINT8            old_sm4;
    tBTM_PAIRING_STATE  old_state   = btm_cb.pairing_state;
    tBTM_SEC_DEV_REC *p_dev_rec = btm_find_dev_by_handle (handle);
    BOOLEAN             are_bonding = FALSE;

  
#if (BT_USE_TRACES == TRUE)
 if (p_dev_rec)
 {
        BTM_TRACE_EVENT ("Security Manager: auth_complete PairState: %s  handle:%u  status:%d  dev->sec_state: %u  Bda:%08x, RName:%s",
                          btm_pair_state_descr (btm_cb.pairing_state),
                          handle, status,
                          p_dev_rec->sec_state,
 (p_dev_rec->bd_addr[2]<<24)+(p_dev_rec->bd_addr[3]<<16)+(p_dev_rec->bd_addr[4]<<8)+p_dev_rec->bd_addr[5],
                          p_dev_rec->sec_bd_name);
 }
 else
 {
        BTM_TRACE_EVENT ("Security Manager: auth_complete PairState: %s  handle:%u  status:%d",
                          btm_pair_state_descr (btm_cb.pairing_state),
                          handle, status);
 }
#endif

  
  
 if ((status == HCI_ERR_LMP_ERR_TRANS_COLLISION) || (status == HCI_ERR_DIFF_TRANSACTION_COLLISION))
 {
        btm_sec_auth_collision(handle);
 return;
 }
    btm_cb.collision_start_time = 0;

    btm_restore_mode();

  
 if (p_dev_rec
 && (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
 && !(btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE))
 {
        p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;

        l2cu_start_post_bond_timer (p_dev_rec->hci_handle);
 }

 if (!p_dev_rec)
 return;

  
    old_sm4 = p_dev_rec->sm4;
    p_dev_rec->sm4 &= ~BTM_SM4_RETRY;

 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
 && (memcmp (p_dev_rec->bd_addr, btm_cb.pairing_bda, BD_ADDR_LEN) == 0) )
        are_bonding = TRUE;

    btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);

 if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING)
 {
 if ( (btm_cb.api.p_auth_complete_callback && status != HCI_SUCCESS)
 && (old_state != BTM_PAIR_STATE_IDLE) )
 {
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                    p_dev_rec->dev_class,
                                                    p_dev_rec->sec_bd_name, status);
 }
 return;
 }

  
 if ((status == HCI_ERR_COMMAND_DISALLOWED)
 && ((p_dev_rec->sec_flags & (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED)) ==
 (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED)))
 {
        status = HCI_SUCCESS;
 }
  
  
 if (btm_cb.api.p_auth_complete_callback)
 {
  
 if (old_state != BTM_PAIR_STATE_IDLE)
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                    p_dev_rec->dev_class,
                                                    p_dev_rec->sec_bd_name, status);
 }

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

  
 if (are_bonding)
 {
        p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;

 if (status != HCI_SUCCESS)
 {
 if(((status != HCI_ERR_PEER_USER) && (status != HCI_ERR_CONN_CAUSE_LOCAL_HOST)))
                btm_sec_send_hci_disconnect (p_dev_rec, HCI_ERR_PEER_USER, p_dev_rec->hci_handle);
 }
 else
            l2cu_start_post_bond_timer (p_dev_rec->hci_handle);

 return;
 }

  
 if (status != HCI_SUCCESS)
 {
 if ((old_sm4 & BTM_SM4_RETRY) == 0)
 {
  
 if (status == HCI_ERR_LMP_ERR_TRANS_COLLISION)
 {
  
                p_dev_rec->sm4 |= BTM_SM4_RETRY;
                BTM_TRACE_DEBUG ("Collision retry sm4:x%x sec_flags:0x%x", p_dev_rec->sm4, p_dev_rec->sec_flags);
 }
  
 else if (HCI_ERR_KEY_MISSING == status && BTM_SEC_IS_SM4(p_dev_rec->sm4))
 {
  
                p_dev_rec->sm4 |= BTM_SM4_RETRY;
                p_dev_rec->sec_flags &= ~BTM_SEC_LINK_KEY_KNOWN;
                BTM_TRACE_DEBUG ("Retry for missing key sm4:x%x sec_flags:0x%x", p_dev_rec->sm4, p_dev_rec->sec_flags);

  
 }

 if (p_dev_rec->sm4 & BTM_SM4_RETRY)
 {
                btm_sec_execute_procedure (p_dev_rec);
 return;
 }
 }

        btm_sec_dev_rec_cback_event (p_dev_rec, BTM_ERR_PROCESSING, FALSE);

 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE)
 {
            btm_sec_send_hci_disconnect (p_dev_rec, HCI_ERR_AUTH_FAILURE, p_dev_rec->hci_handle);
 }
 return;
 }

    p_dev_rec->sec_flags |= BTM_SEC_AUTHENTICATED;

  
    status = btm_sec_execute_procedure (p_dev_rec);

  
 if (status != BTM_CMD_STARTED)
        btm_sec_dev_rec_cback_event (p_dev_rec, status, FALSE);
}
