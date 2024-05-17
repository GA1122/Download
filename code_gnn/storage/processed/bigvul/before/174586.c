void btm_rem_oob_req (UINT8 *p)
{
    UINT8 *p_bda;
    tBTM_SP_RMT_OOB  evt_data;
    tBTM_SEC_DEV_REC *p_dev_rec;
    BT_OCTET16      c;
    BT_OCTET16      r;

    p_bda = evt_data.bd_addr;

    STREAM_TO_BDADDR (p_bda, p);

    BTM_TRACE_EVENT ("btm_rem_oob_req() BDA: %02x:%02x:%02x:%02x:%02x:%02x",
                      p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

 if ( (NULL != (p_dev_rec = btm_find_dev (p_bda))) &&
         btm_cb.api.p_sp_callback)
 {
        memcpy (evt_data.bd_addr, p_dev_rec->bd_addr, BD_ADDR_LEN);
        memcpy (evt_data.dev_class, p_dev_rec->dev_class, DEV_CLASS_LEN);
        BCM_STRNCPY_S((char *)evt_data.bd_name, sizeof(evt_data.bd_name), (char *)p_dev_rec->sec_bd_name, BTM_MAX_REM_BD_NAME_LEN+1);
        evt_data.bd_name[BTM_MAX_REM_BD_NAME_LEN] = 0;

        btm_sec_change_pairing_state(BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP);
 if ((*btm_cb.api.p_sp_callback) (BTM_SP_RMT_OOB_EVT, (tBTM_SP_EVT_DATA *)&evt_data) == BTM_NOT_AUTHORIZED)
 {
            BTM_RemoteOobDataReply(TRUE, p_bda, c, r);
 }
 return;
 }

  
    btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;
    btsnd_hcic_rem_oob_neg_reply (p_bda);
}
