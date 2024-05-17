void btm_sec_connected (UINT8 *bda, UINT16 handle, UINT8 status, UINT8 enc_mode)
{
    tBTM_SEC_DEV_REC *p_dev_rec = btm_find_dev (bda);
    UINT8            res;
    BOOLEAN          is_pairing_device = FALSE;
    tACL_CONN        *p_acl_cb;
    UINT8            bit_shift = 0;

    btm_acl_resubmit_page();

  
#if (BT_USE_TRACES == TRUE)
 if (p_dev_rec)
 {
        BTM_TRACE_EVENT ("Security Manager: btm_sec_connected in state: %s  handle:%d status:%d enc_mode:%d  bda:%x RName:%s",
                          btm_pair_state_descr(btm_cb.pairing_state), handle, status, enc_mode,
 (bda[2]<<24)+(bda[3]<<16)+(bda[4]<<8)+bda[5],
                          p_dev_rec->sec_bd_name);
 }
 else
 {
        BTM_TRACE_EVENT ("Security Manager: btm_sec_connected in state: %s  handle:%d status:%d enc_mode:%d  bda:%x ",
                          btm_pair_state_descr(btm_cb.pairing_state), handle, status, enc_mode,
 (bda[2]<<24)+(bda[3]<<16)+(bda[4]<<8)+bda[5]);
 }
#endif

 if (!p_dev_rec)
 {
  
 if (status == HCI_SUCCESS)
 {
            p_dev_rec = btm_sec_alloc_dev (bda);
 }
 else
 {
  
 return;
 }
 }
 else  
 {

#if BLE_INCLUDED == TRUE
        bit_shift = (handle == p_dev_rec->ble_hci_handle) ? 8 :0;
#endif
        p_dev_rec->timestamp = btm_cb.dev_rec_count++;
 if (p_dev_rec->sm4 & BTM_SM4_CONN_PEND)
 {
  
 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (memcmp (btm_cb.pairing_bda, p_dev_rec->bd_addr, BD_ADDR_LEN) == 0)
 && (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) )
 {
  
  
 if ((status != HCI_SUCCESS)&&(btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_PIN_REQ))
 {
                    BTM_TRACE_WARNING ("Security Manager: btm_sec_connected: incoming connection failed without asking PIN");

                    p_dev_rec->sm4 &= ~BTM_SM4_CONN_PEND;
 if (p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN)
 {
  
  
                        btm_cb.p_collided_dev_rec = p_dev_rec;
                        btm_cb.sec_collision_tle.param = (UINT32) btm_sec_connect_after_reject_timeout;
                        btu_start_timer (&btm_cb.sec_collision_tle, BTU_TTYPE_USER_FUNC, 0);
 }
 else
 {
                        btm_sec_change_pairing_state (BTM_PAIR_STATE_GET_REM_NAME);
                        BTM_ReadRemoteDeviceName(p_dev_rec->bd_addr, NULL, BT_TRANSPORT_BR_EDR);
 }
#if BTM_DISC_DURING_RS == TRUE
                    p_dev_rec->rs_disc_pending   = BTM_SEC_RS_NOT_PENDING;  
#endif
 return;
 }
 else
 {
                    l2cu_update_lcb_4_bonding(p_dev_rec->bd_addr, TRUE);
 }
 }
  
            p_dev_rec->sm4 &= ~BTM_SM4_CONN_PEND;
 }
 }

#if BLE_INCLUDED == TRUE
    p_dev_rec->device_type |= BT_DEVICE_TYPE_BREDR;
#endif

#if BTM_DISC_DURING_RS == TRUE
    p_dev_rec->rs_disc_pending   = BTM_SEC_RS_NOT_PENDING;  
#endif

    p_dev_rec->rs_disc_pending   = BTM_SEC_RS_NOT_PENDING;  

 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (memcmp (btm_cb.pairing_bda, bda, BD_ADDR_LEN) == 0) )
 {
  
 if ((status == HCI_ERR_HOST_REJECT_DEVICE)
 &&(btm_cb.pairing_flags & BTM_PAIR_FLAGS_REJECTED_CONNECT))
 {
            BTM_TRACE_WARNING ("Security Manager: btm_sec_connected: HCI_Conn_Comp Flags:0x%04x, sm4: 0x%x",
                btm_cb.pairing_flags, p_dev_rec->sm4);

            btm_cb.pairing_flags &= ~BTM_PAIR_FLAGS_REJECTED_CONNECT;
 if (BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4))
 {
  
                btm_sec_change_pairing_state (BTM_PAIR_STATE_GET_REM_NAME);
                BTM_ReadRemoteDeviceName(bda, NULL, BT_TRANSPORT_BR_EDR);
 return;
 }

  
 if (btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_LOCAL_PIN)
 {
  
  
                btm_cb.p_collided_dev_rec = p_dev_rec;
                btm_cb.sec_collision_tle.param = (UINT32) btm_sec_connect_after_reject_timeout;
                btu_start_timer (&btm_cb.sec_collision_tle, BTU_TTYPE_USER_FUNC, 0);
 }

 return;
 }
  
 else if (status == HCI_ERR_CONNECTION_EXISTS)
 {
            BTM_TRACE_WARNING ("Security Manager: btm_sec_connected: Wait for incoming connection");
 return;
 }

        is_pairing_device = TRUE;
 }

  
    btm_restore_mode();

  
 if (status != HCI_SUCCESS)
 {
  
 if (is_pairing_device)
 {
            p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;
            p_dev_rec->sec_flags &= ~((BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_LINK_KEY_AUTHED) << bit_shift);
            BTM_TRACE_DEBUG ("security_required:%x ", p_dev_rec->security_required );

            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);

  
 if (btm_cb.api.p_auth_complete_callback)
 {
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                        p_dev_rec->dev_class,
                                                        p_dev_rec->sec_bd_name, status);
 }
 }
  
 else if ((p_dev_rec->link_key_type  <= BTM_LKEY_TYPE_REMOTE_UNIT) &&
 (((status == HCI_ERR_AUTH_FAILURE) ||
 (status == HCI_ERR_KEY_MISSING) ||
 (status == HCI_ERR_HOST_REJECT_SECURITY) ||
 (status == HCI_ERR_PAIRING_NOT_ALLOWED) ||
 (status == HCI_ERR_UNIT_KEY_USED) ||
 (status == HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED) ||
 (status == HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE) ||
 (status == HCI_ERR_REPEATED_ATTEMPTS))))
 {
            p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;
            p_dev_rec->sec_flags &= ~ (BTM_SEC_LE_LINK_KEY_KNOWN << bit_shift);


#ifdef BRCM_NOT_4_BTE
  
 if (btm_cb.acl_disc_reason == HCI_ERR_HOST_REJECT_SECURITY)
 {
                status = HCI_ERR_HOST_REJECT_SECURITY;
 }
#endif

  
 if (btm_cb.api.p_auth_complete_callback)
 {
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                        p_dev_rec->dev_class,
                                                        p_dev_rec->sec_bd_name, status);
 }
 }

 if (status == HCI_ERR_CONNECTION_TOUT || status == HCI_ERR_LMP_RESPONSE_TIMEOUT  ||
            status == HCI_ERR_UNSPECIFIED     || status == HCI_ERR_PAGE_TIMEOUT)
            btm_sec_dev_rec_cback_event (p_dev_rec, BTM_DEVICE_TIMEOUT, FALSE);
 else
            btm_sec_dev_rec_cback_event (p_dev_rec, BTM_ERR_PROCESSING, FALSE);

 return;
 }

  
  
 if ( is_pairing_device
 && (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) )
 {
 if (p_dev_rec->link_key_not_sent)
 {
            p_dev_rec->link_key_not_sent = FALSE;
            btm_send_link_key_notif(p_dev_rec);
 }

        p_dev_rec->security_required &= ~BTM_SEC_OUT_AUTHENTICATE;

  
 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
            res = TRUE;
 else
            res = FALSE;

 if (btm_cb.api.p_auth_complete_callback)
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                    p_dev_rec->dev_class,
                                                    p_dev_rec->sec_bd_name, HCI_SUCCESS);

        btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);

 if ( res )
 {
  
            l2cu_update_lcb_4_bonding (p_dev_rec->bd_addr, TRUE);
 }

 return;
 }

    p_dev_rec->hci_handle = handle;

  
  
  
    p_acl_cb = btm_bda_to_acl(bda, BT_TRANSPORT_BR_EDR);
 if (p_acl_cb)
 {
  
#if (!defined(BTM_BYPASS_EXTRA_ACL_SETUP) || BTM_BYPASS_EXTRA_ACL_SETUP == FALSE)
  
  
  
        btm_set_packet_types (p_acl_cb, btm_cb.btm_acl_pkt_types_supported);

 if (btm_cb.btm_def_link_policy)
            BTM_SetLinkPolicy (p_acl_cb->remote_addr, &btm_cb.btm_def_link_policy);
#endif
 }
    btm_acl_created (bda, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, handle, HCI_ROLE_SLAVE, BT_TRANSPORT_BR_EDR);

  
  
  
    p_dev_rec->sec_flags &= ~((BTM_SEC_AUTHORIZED | BTM_SEC_AUTHENTICATED |
                              BTM_SEC_ENCRYPTED | BTM_SEC_ROLE_SWITCHED) << bit_shift);

 if (enc_mode != HCI_ENCRYPT_MODE_DISABLED)
        p_dev_rec->sec_flags |= ((BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED) << bit_shift);

 if (btm_cb.security_mode == BTM_SEC_MODE_LINK)
        p_dev_rec->sec_flags |= (BTM_SEC_AUTHENTICATED << bit_shift);

    p_dev_rec->link_key_changed = FALSE;

  
  
  
    BTM_TRACE_DEBUG ("is_originator:%d ", p_dev_rec->is_originator);
 if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) || p_dev_rec->is_originator)
 {
 if ((res = btm_sec_execute_procedure (p_dev_rec)) != BTM_CMD_STARTED)
            btm_sec_dev_rec_cback_event (p_dev_rec, res, FALSE);
 }
 return;
}
