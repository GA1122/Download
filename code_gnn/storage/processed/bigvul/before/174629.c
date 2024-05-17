void btm_sec_rmt_name_request_complete (UINT8 *p_bd_addr, UINT8 *p_bd_name, UINT8 status)
{
    tBTM_SEC_DEV_REC *p_dev_rec;
 int              i;
    DEV_CLASS        dev_class;
    UINT8            old_sec_state;

    BTM_TRACE_EVENT ("btm_sec_rmt_name_request_complete");
 if (((p_bd_addr == NULL) && !BTM_ACL_IS_CONNECTED(btm_cb.connecting_bda))
 || ((p_bd_addr != NULL) && !BTM_ACL_IS_CONNECTED(p_bd_addr)))
 {
        btm_acl_resubmit_page();
 }

  
  
 if (p_bd_addr)
        p_dev_rec = btm_find_dev (p_bd_addr);
 else
 {
        p_dev_rec = &btm_cb.sec_dev_rec[0];

 for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++)
 {
 if ((p_dev_rec->sec_flags & BTM_SEC_IN_USE)
 && (p_dev_rec->sec_state == BTM_SEC_STATE_GETTING_NAME))
 {
                p_bd_addr = p_dev_rec->bd_addr;
 break;
 }
 }

 if (i == BTM_SEC_MAX_DEVICE_RECORDS)
            p_dev_rec = NULL;
 }


  
#if (BT_USE_TRACES == TRUE)
 if (!p_bd_name)
        p_bd_name = (UINT8 *)"";

 if (p_dev_rec)
 {
        BTM_TRACE_EVENT ("Security Manager: rmt_name_complete PairState: %s  RemName: %s  status: %d State:%d  p_dev_rec: 0x%08x ",
                          btm_pair_state_descr (btm_cb.pairing_state), p_bd_name,
                          status, p_dev_rec->sec_state, p_dev_rec);
 }
 else
 {
        BTM_TRACE_EVENT ("Security Manager: rmt_name_complete PairState: %s  RemName: %s  status: %d",
                          btm_pair_state_descr (btm_cb.pairing_state), p_bd_name,
                          status);
 }
#endif

 if (p_dev_rec)
 {
        old_sec_state = p_dev_rec->sec_state;
 if (status == HCI_SUCCESS)
 {
            BCM_STRNCPY_S ((char *)p_dev_rec->sec_bd_name, sizeof (p_dev_rec->sec_bd_name), (char *)p_bd_name, BTM_MAX_REM_BD_NAME_LEN);
            p_dev_rec->sec_flags |= BTM_SEC_NAME_KNOWN;
            BTM_TRACE_EVENT ("setting BTM_SEC_NAME_KNOWN sec_flags:0x%x", p_dev_rec->sec_flags);
 }
 else
 {
  
            p_dev_rec->sec_bd_name[0] = 0;
 }

 if (p_dev_rec->sec_state == BTM_SEC_STATE_GETTING_NAME)
            p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

  
 for (i = 0;i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
 {
 if (btm_cb.p_rmt_name_callback[i] && p_bd_addr)
 (*btm_cb.p_rmt_name_callback[i])(p_bd_addr, p_dev_rec->dev_class,
                                                 p_dev_rec->sec_bd_name);
 }
 }
 else
 {
        dev_class[0] = 0;
        dev_class[1] = 0;
        dev_class[2] = 0;

  
 for (i = 0;i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
 {
 if (btm_cb.p_rmt_name_callback[i] && p_bd_addr)
 (*btm_cb.p_rmt_name_callback[i])(p_bd_addr, dev_class, (UINT8 *)"");
 }

 return;
 }

  
 if ( (btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_LOCAL_PIN) && p_bd_addr
 && (memcmp (btm_cb.pairing_bda, p_bd_addr, BD_ADDR_LEN) == 0) )
 {
        BTM_TRACE_EVENT ("btm_sec_rmt_name_request_complete() delayed pin now being requested flags:0x%x, (p_pin_callback=0x%p)", btm_cb.pairing_flags, btm_cb.api.p_pin_callback);

 if (((btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) == 0) &&
 ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_PIN_REQD) == 0) &&
            btm_cb.api.p_pin_callback)
 {
            BTM_TRACE_EVENT ("btm_sec_rmt_name_request_complete() calling pin_callback");
            btm_cb.pairing_flags |= BTM_PAIR_FLAGS_PIN_REQD;
 (*btm_cb.api.p_pin_callback) (p_dev_rec->bd_addr, p_dev_rec->dev_class, p_bd_name);
 }

  
        btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_LOCAL_PIN);
 return;
 }

  
 if ( btm_cb.pairing_state == BTM_PAIR_STATE_GET_REM_NAME)
 {
 if (p_bd_addr && memcmp (btm_cb.pairing_bda, p_bd_addr, BD_ADDR_LEN) == 0)
 {
            BTM_TRACE_EVENT ("btm_sec_rmt_name_request_complete() continue bonding sm4: 0x%04x, status:0x%x", p_dev_rec->sm4, status);
 if(btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_CANCEL_DD)
 {
                btm_sec_bond_cancel_complete();
 return;
 }

 if (status != HCI_SUCCESS)
 {
                btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);

 if (btm_cb.api.p_auth_complete_callback)
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,  p_dev_rec->dev_class,
                                                            p_dev_rec->sec_bd_name, status);
 return;
 }

  
 if (BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4))
 {
  
  
                BTM_TRACE_DEBUG ("btm_sec_rmt_name_request_complete  IS_SM4_UNKNOWN Flags:0x%04x",
                                   btm_cb.pairing_flags);
 if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_REJECTED_CONNECT) == 0)
 {
                    p_dev_rec->sm4 |= BTM_SM4_KNOWN;
 }
 }

            BTM_TRACE_DEBUG("%s, SM4 Value: %x, Legacy:%d,IS SM4:%d, Unknown:%d",__FUNCTION__,
                p_dev_rec->sm4, BTM_SEC_IS_SM4_LEGACY(p_dev_rec->sm4),
                BTM_SEC_IS_SM4(p_dev_rec->sm4),BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4));

  
 if ((p_dev_rec->sm4 != BTM_SM4_KNOWN) || !btm_sec_check_prefetch_pin(p_dev_rec))
 {
  
  
 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_REJECTED_CONNECT)
 {
                    BTM_TRACE_WARNING ("btm_sec_rmt_name_request_complete: waiting HCI_Connection_Complete after rejecting connection");
 }
  
 else if (btm_sec_dd_create_conn(p_dev_rec) != BTM_CMD_STARTED)
 {
                    BTM_TRACE_WARNING ("btm_sec_rmt_name_request_complete: failed to start connection");

                    btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);

 if (btm_cb.api.p_auth_complete_callback)
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,  p_dev_rec->dev_class,
                                                            p_dev_rec->sec_bd_name, HCI_ERR_MEMORY_FULL);
 }
 }
 return;
 }
 else
 {
            BTM_TRACE_WARNING ("btm_sec_rmt_name_request_complete: wrong BDA, retry with pairing BDA");

            BTM_ReadRemoteDeviceName (btm_cb.pairing_bda, NULL, BT_TRANSPORT_BR_EDR);
 return;
 }
 }

  
 if (p_dev_rec->link_key_not_sent)
 {
  
 if (p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE)
 return;

        p_dev_rec->link_key_not_sent = FALSE;
        btm_send_link_key_notif(p_dev_rec);

  
  
  
 if (!(p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE))
 {
 if (btm_cb.api.p_auth_complete_callback)
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                        p_dev_rec->dev_class,
                                                        p_dev_rec->sec_bd_name, HCI_SUCCESS);

 }
 }

  
 if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
 && (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))
 {
        BTM_TRACE_WARNING ("btm_sec_rmt_name_request_complete (none/ce)");
        p_dev_rec->security_required &= ~(BTM_SEC_OUT_AUTHENTICATE);
        l2cu_start_post_bond_timer(p_dev_rec->hci_handle);
 return;
 }

 if (old_sec_state != BTM_SEC_STATE_GETTING_NAME)
 return;

  
 if (status != HCI_SUCCESS)
 {
        btm_sec_dev_rec_cback_event  (p_dev_rec, BTM_ERR_PROCESSING, FALSE);
 return;
 }

 if (p_dev_rec->sm4 & BTM_SM4_REQ_PEND)
 {
        BTM_TRACE_EVENT ("waiting for remote features!!");
 return;
 }

  
    status = (UINT8)btm_sec_execute_procedure (p_dev_rec);

  
 if (status == BTM_CMD_STARTED)
 return;

  
    btm_sec_dev_rec_cback_event  (p_dev_rec, status, FALSE);
}
