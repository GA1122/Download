void btm_proc_sp_req_evt (tBTM_SP_EVT event, UINT8 *p)
{
    tBTM_STATUS status = BTM_ERR_PROCESSING;
    tBTM_SP_EVT_DATA evt_data;
    UINT8               *p_bda = evt_data.cfm_req.bd_addr;
    tBTM_SEC_DEV_REC *p_dev_rec;

  
    STREAM_TO_BDADDR (p_bda, p);

    BTM_TRACE_EVENT ("btm_proc_sp_req_evt() BDA: %08x%04x event: 0x%x, State: %s",
 (p_bda[0]<<24) + (p_bda[1]<<16) + (p_bda[2]<<8) + p_bda[3], (p_bda[4] << 8) + p_bda[5],
                      event, btm_pair_state_descr(btm_cb.pairing_state));

 if ( ((p_dev_rec = btm_find_dev (p_bda)) != NULL)
 && (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (memcmp (btm_cb.pairing_bda, p_bda, BD_ADDR_LEN) == 0) )
 {
        memcpy (evt_data.cfm_req.bd_addr, p_dev_rec->bd_addr, BD_ADDR_LEN);
        memcpy (evt_data.cfm_req.dev_class, p_dev_rec->dev_class, DEV_CLASS_LEN);

        BCM_STRNCPY_S ((char *)evt_data.cfm_req.bd_name, sizeof(evt_data.cfm_req.bd_name), (char *)p_dev_rec->sec_bd_name, BTM_MAX_REM_BD_NAME_LEN);

 switch (event)
 {
 case BTM_SP_CFM_REQ_EVT:
  
                btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM);

  
                STREAM_TO_UINT32 (evt_data.cfm_req.num_val, p);

                evt_data.cfm_req.just_works = TRUE;

  
#if (BTM_LOCAL_IO_CAPS == BTM_IO_CAP_IO)
 if ( (p_dev_rec->rmt_io_caps == BTM_IO_CAP_IO)
 && (btm_cb.devcb.loc_io_caps == BTM_IO_CAP_IO)
 && ((p_dev_rec->rmt_auth_req & BTM_AUTH_SP_YES) || (btm_cb.devcb.loc_auth_req & BTM_AUTH_SP_YES)) )
 {
  
                    evt_data.cfm_req.just_works = FALSE;
 }
#endif
                BTM_TRACE_DEBUG ("btm_proc_sp_req_evt()  just_works:%d, io loc:%d, rmt:%d, auth loc:%d, rmt:%d",
                                  evt_data.cfm_req.just_works, btm_cb.devcb.loc_io_caps, p_dev_rec->rmt_io_caps,
                                  btm_cb.devcb.loc_auth_req, p_dev_rec->rmt_auth_req);

                evt_data.cfm_req.loc_auth_req   = btm_cb.devcb.loc_auth_req;
                evt_data.cfm_req.rmt_auth_req   = p_dev_rec->rmt_auth_req;
                evt_data.cfm_req.loc_io_caps    = btm_cb.devcb.loc_io_caps;
                evt_data.cfm_req.rmt_io_caps    = p_dev_rec->rmt_io_caps;
 break;

 case BTM_SP_KEY_NOTIF_EVT:
  
                STREAM_TO_UINT32 (evt_data.key_notif.passkey, p);

                BTM_TRACE_DEBUG ("BTM_SP_KEY_NOTIF_EVT:  passkey: %u", evt_data.key_notif.passkey);

                btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
 break;

#if (BTM_LOCAL_IO_CAPS != BTM_IO_CAP_NONE)
 case BTM_SP_KEY_REQ_EVT:
  
                btm_sec_change_pairing_state (BTM_PAIR_STATE_KEY_ENTRY);
 break;
#endif
 }

 if (btm_cb.api.p_sp_callback)
 {
            status = (*btm_cb.api.p_sp_callback) (event, (tBTM_SP_EVT_DATA *)&evt_data);
 if (status != BTM_NOT_AUTHORIZED)
 {
 return;
 }
  
 }
 else if ( (event == BTM_SP_CFM_REQ_EVT) && (evt_data.cfm_req.just_works == TRUE) )
 {
  
            status = BTM_SUCCESS;
 }

 if (event == BTM_SP_CFM_REQ_EVT)
 {
            BTM_TRACE_DEBUG ("calling BTM_ConfirmReqReply with status: %d", status);
            BTM_ConfirmReqReply (status, p_bda);
 }
#if (BTM_LOCAL_IO_CAPS != BTM_IO_CAP_NONE)
 else if (event == BTM_SP_KEY_REQ_EVT)
 {
            BTM_PasskeyReqReply(status, p_bda, 0);
 }
#endif
 return;
 }

  
    btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;

 if (BTM_SP_CFM_REQ_EVT == event)
 {
        btsnd_hcic_user_conf_reply (p_bda, FALSE);
 }
 else if (BTM_SP_KEY_NOTIF_EVT == event)
 {
  
 if (NULL != (p_dev_rec = btm_find_dev (p_bda)) )
 {
            btm_sec_disconnect (p_dev_rec->hci_handle, HCI_ERR_AUTH_FAILURE);
 }
 }
#if (BTM_LOCAL_IO_CAPS != BTM_IO_CAP_NONE)
 else
 {
        btsnd_hcic_user_passkey_neg_reply(p_bda);
 }
#endif
}
