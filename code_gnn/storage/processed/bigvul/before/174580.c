void btm_io_capabilities_req (UINT8 *p)
{
    tBTM_SP_IO_REQ  evt_data;
    UINT8           err_code = 0;
    tBTM_SEC_DEV_REC *p_dev_rec;
    BOOLEAN         is_orig = TRUE;
    UINT8           callback_rc = BTM_SUCCESS;

    STREAM_TO_BDADDR (evt_data.bd_addr, p);

  
  
    evt_data.io_cap = btm_cb.devcb.loc_io_caps;
    evt_data.oob_data = BTM_OOB_NONE;
    evt_data.auth_req = BTM_DEFAULT_AUTH_REQ;

    BTM_TRACE_EVENT ("btm_io_capabilities_req() State: %s", btm_pair_state_descr(btm_cb.pairing_state));

    p_dev_rec = btm_find_or_alloc_dev (evt_data.bd_addr);
    p_dev_rec->sm4 |= BTM_SM4_TRUE;

    BTM_TRACE_EVENT ("btm_io_capabilities_req() State: %s  Flags: 0x%04x  p_cur_service: 0x%08x",
                      btm_pair_state_descr(btm_cb.pairing_state), btm_cb.pairing_flags, p_dev_rec->p_cur_service);

 if (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 {
 if (btm_cb.pairing_state == BTM_PAIR_STATE_INCOMING_SSP)
 {
  
            is_orig = FALSE;

 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_PEER_STARTED_DD)
                evt_data.auth_req = BTM_DEFAULT_DD_AUTH_REQ;
 }
  
 else if (btm_cb.pairing_state == BTM_PAIR_STATE_WAIT_PIN_REQ)
 {
 
 if (memcmp (evt_data.bd_addr, btm_cb.pairing_bda, BD_ADDR_LEN))
 {
  
                err_code = HCI_ERR_HOST_BUSY_PAIRING;
 }
 else
 {
  
                evt_data.auth_req = BTM_DEFAULT_DD_AUTH_REQ;
 }
 }
 else
 {
            err_code = HCI_ERR_HOST_BUSY_PAIRING;
 }
 }

  
 if (btm_cb.pairing_disabled)
        err_code = HCI_ERR_PAIRING_NOT_ALLOWED;

 if (err_code != 0)
 {
 
        btsnd_hcic_io_cap_req_neg_reply(evt_data.bd_addr, err_code);
 return;
 }

    evt_data.is_orig = is_orig;

 if (is_orig)
 {
  
 if (!(btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) &&
            p_dev_rec->p_cur_service &&
 (p_dev_rec->p_cur_service->security_flags & BTM_SEC_OUT_AUTHENTICATE))
 {
            evt_data.auth_req = (p_dev_rec->p_cur_service->security_flags & BTM_SEC_OUT_MITM) ? BTM_AUTH_SP_YES : BTM_AUTH_SP_NO;
 }
 }

  
    l2c_pin_code_request (evt_data.bd_addr);

    memcpy (btm_cb.pairing_bda, evt_data.bd_addr, BD_ADDR_LEN);

 
 if (!memcmp (evt_data.bd_addr, btm_cb.connecting_bda, BD_ADDR_LEN))
        memcpy (p_dev_rec->dev_class, btm_cb.connecting_dc, DEV_CLASS_LEN);

    btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS);

    callback_rc = BTM_SUCCESS;
 if (p_dev_rec->sm4 & BTM_SM4_UPGRADE)
 {
        p_dev_rec->sm4 &= ~BTM_SM4_UPGRADE;

  
        evt_data.auth_req = BTM_AUTH_SPGB_YES;
 }
 else if (btm_cb.api.p_sp_callback)
 {
  
        callback_rc = (*btm_cb.api.p_sp_callback) (BTM_SP_IO_REQ_EVT, (tBTM_SP_EVT_DATA *)&evt_data);
 }

#if BTM_OOB_INCLUDED == TRUE
 if ((callback_rc == BTM_SUCCESS) || (BTM_OOB_UNKNOWN != evt_data.oob_data))
#else
 if (callback_rc == BTM_SUCCESS)
#endif
 {
 if ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD))
 {
            evt_data.auth_req = (BTM_AUTH_DD_BOND | (evt_data.auth_req & BTM_AUTH_YN_BIT));
 }

  
        btm_cb.devcb.loc_auth_req   = evt_data.auth_req;
        btm_cb.devcb.loc_io_caps    = evt_data.io_cap;

        BTM_TRACE_EVENT ("btm_io_capabilities_req: State: %s  IO_CAP:%d oob_data:%d auth_req:%d",
                          btm_pair_state_descr(btm_cb.pairing_state), evt_data.io_cap,
                          evt_data.oob_data, evt_data.auth_req);

        btsnd_hcic_io_cap_req_reply(evt_data.bd_addr, evt_data.io_cap,
                                    evt_data.oob_data, evt_data.auth_req);
 }
}
