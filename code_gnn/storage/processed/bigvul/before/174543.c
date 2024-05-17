void BTM_ConfirmReqReply(tBTM_STATUS res, BD_ADDR bd_addr)
{
    tBTM_SEC_DEV_REC *p_dev_rec;

    BTM_TRACE_EVENT ("BTM_ConfirmReqReply() State: %s  Res: %u",
                      btm_pair_state_descr(btm_cb.pairing_state), res);

  
 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM)
 || (memcmp (btm_cb.pairing_bda, bd_addr, BD_ADDR_LEN) != 0) )
 return;

    btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);

 if ( (res == BTM_SUCCESS) || (res == BTM_SUCCESS_NO_SECURITY) )
 {
        btm_cb.acl_disc_reason = HCI_SUCCESS;

 if (res == BTM_SUCCESS)
 {
 if ((p_dev_rec = btm_find_dev (bd_addr)) != NULL)
                p_dev_rec->sec_flags |= BTM_SEC_LINK_KEY_AUTHED;
 }

        btsnd_hcic_user_conf_reply (bd_addr, TRUE);
 }
 else
 {
  
        btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;
        btsnd_hcic_user_conf_reply (bd_addr, FALSE);
 }
}
