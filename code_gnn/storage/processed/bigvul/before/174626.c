static void btm_sec_pairing_timeout (TIMER_LIST_ENT *p_tle)
{
    tBTM_CB *p_cb = &btm_cb;
    tBTM_SEC_DEV_REC *p_dev_rec;
#if BTM_OOB_INCLUDED == TRUE
#if (BTM_LOCAL_IO_CAPS == BTM_IO_CAP_NONE)
    tBTM_AUTH_REQ   auth_req = BTM_AUTH_AP_NO;
#else
    tBTM_AUTH_REQ   auth_req = BTM_AUTH_AP_YES;
#endif
#endif
    UINT8   name[2];
    UNUSED(p_tle);

    p_cb->pairing_tle.param = 0;
 
 
    p_dev_rec = btm_find_dev (p_cb->pairing_bda);

    BTM_TRACE_EVENT ("btm_sec_pairing_timeout()  State: %s   Flags: %u",
                      btm_pair_state_descr(p_cb->pairing_state), p_cb->pairing_flags);

 switch (p_cb->pairing_state)
 {
 case BTM_PAIR_STATE_WAIT_PIN_REQ:
            btm_sec_bond_cancel_complete();
 break;

 case BTM_PAIR_STATE_WAIT_LOCAL_PIN:
 if ( (btm_cb.pairing_flags & BTM_PAIR_FLAGS_PRE_FETCH_PIN) == 0)
                btsnd_hcic_pin_code_neg_reply (p_cb->pairing_bda);
            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
  
 if (btm_cb.api.p_auth_complete_callback)
 {
 if (p_dev_rec == NULL)
 {
                    name[0] = 0;
 (*btm_cb.api.p_auth_complete_callback) (p_cb->pairing_bda,
                                                            NULL,
                                                            name, HCI_ERR_CONNECTION_TOUT);
 }
 else
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                            p_dev_rec->dev_class,
                                                            p_dev_rec->sec_bd_name, HCI_ERR_CONNECTION_TOUT);
 }
 break;

 case BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM:
            btsnd_hcic_user_conf_reply (p_cb->pairing_bda, FALSE);
  
 break;

#if (BTM_LOCAL_IO_CAPS != BTM_IO_CAP_NONE)
 case BTM_PAIR_STATE_KEY_ENTRY:
            btsnd_hcic_user_passkey_neg_reply(p_cb->pairing_bda);
  
 break;
#endif  

#if BTM_OOB_INCLUDED == TRUE
 case BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS:
 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
                auth_req |= BTM_AUTH_DD_BOND;

            btsnd_hcic_io_cap_req_reply (p_cb->pairing_bda, btm_cb.devcb.loc_io_caps,
                                         BTM_OOB_NONE, auth_req);
            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
 break;

 case BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP:
            btsnd_hcic_rem_oob_neg_reply (p_cb->pairing_bda);
            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
 break;
#endif  

 case BTM_PAIR_STATE_WAIT_DISCONNECT:
  
 if (p_dev_rec == NULL)
 {
                BTM_TRACE_ERROR ("btm_sec_pairing_timeout() BTM_PAIR_STATE_WAIT_DISCONNECT unknown BDA: %08x%04x",
 (p_cb->pairing_bda[0]<<24) + (p_cb->pairing_bda[1]<<16) + (p_cb->pairing_bda[2]<<8) + p_cb->pairing_bda[3],
 (p_cb->pairing_bda[4] << 8) + p_cb->pairing_bda[5]);
 break;
 }
            btm_sec_send_hci_disconnect (p_dev_rec, HCI_ERR_AUTH_FAILURE, p_dev_rec->hci_handle);
            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
 break;

 case BTM_PAIR_STATE_WAIT_AUTH_COMPLETE:
  
            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
 if (btm_cb.api.p_auth_complete_callback)
 {
 if (p_dev_rec == NULL)
 {
                    name[0] = 0;
 (*btm_cb.api.p_auth_complete_callback) (p_cb->pairing_bda,
                                                            NULL,
                                                            name, HCI_ERR_CONNECTION_TOUT);
 }
 else
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr,
                                                            p_dev_rec->dev_class,
                                                            p_dev_rec->sec_bd_name, HCI_ERR_CONNECTION_TOUT);
 }
 break;

 default:
            BTM_TRACE_WARNING ("btm_sec_pairing_timeout() not processed state: %s", btm_pair_state_descr(btm_cb.pairing_state));
            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
 break;
 }
}
