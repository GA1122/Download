void BTM_RemoteOobDataReply(tBTM_STATUS res, BD_ADDR bd_addr, BT_OCTET16 c, BT_OCTET16 r)
{
    BTM_TRACE_EVENT ("BTM_RemoteOobDataReply():  State: %s  res:%d",
                      btm_pair_state_descr(btm_cb.pairing_state), res);

  
 if (btm_cb.pairing_state != BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP)
 return;

    btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);

 if (res != BTM_SUCCESS)
 {
  
        btm_cb.acl_disc_reason = HCI_ERR_HOST_REJECT_SECURITY;
        btsnd_hcic_rem_oob_neg_reply (bd_addr);
 }
 else
 {
        btm_cb.acl_disc_reason = HCI_SUCCESS;
        btsnd_hcic_rem_oob_reply (bd_addr, c, r);
 }
}
