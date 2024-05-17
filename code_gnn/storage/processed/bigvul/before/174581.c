void btm_io_capabilities_rsp (UINT8 *p)
{
    tBTM_SEC_DEV_REC *p_dev_rec;
    tBTM_SP_IO_RSP evt_data;

    STREAM_TO_BDADDR (evt_data.bd_addr, p);
    STREAM_TO_UINT8 (evt_data.io_cap, p);
    STREAM_TO_UINT8 (evt_data.oob_data, p);
    STREAM_TO_UINT8 (evt_data.auth_req, p);

  
    p_dev_rec = btm_find_or_alloc_dev (evt_data.bd_addr);

  
 if (btm_cb.pairing_state == BTM_PAIR_STATE_IDLE)
 {
        memcpy (btm_cb.pairing_bda, evt_data.bd_addr, BD_ADDR_LEN);

        btm_sec_change_pairing_state (BTM_PAIR_STATE_INCOMING_SSP);

  
        BTM_SEC_CLR_TRUSTED_DEVICE(p_dev_rec->trusted_mask);

  
        btm_inq_stop_on_ssp();
 }

  
    l2c_pin_code_request (evt_data.bd_addr);

  
 
 if (!memcmp (evt_data.bd_addr, btm_cb.connecting_bda, BD_ADDR_LEN))
        memcpy (p_dev_rec->dev_class, btm_cb.connecting_dc, DEV_CLASS_LEN);

  
 if (btm_cb.pairing_state == BTM_PAIR_STATE_INCOMING_SSP  
 && (evt_data.auth_req & BTM_AUTH_DD_BOND) )  
 {
        btm_cb.pairing_flags |= BTM_PAIR_FLAGS_PEER_STARTED_DD;
 }

  
    p_dev_rec->rmt_io_caps  = evt_data.io_cap;
    p_dev_rec->rmt_auth_req = evt_data.auth_req;

 if (btm_cb.api.p_sp_callback)
 (*btm_cb.api.p_sp_callback) (BTM_SP_IO_RSP_EVT, (tBTM_SP_EVT_DATA *)&evt_data);
}
