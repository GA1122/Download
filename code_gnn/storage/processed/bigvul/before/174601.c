void btm_sec_conn_req (UINT8 *bda, UINT8 *dc)
{
    tBTM_SEC_DEV_REC  *p_dev_rec = btm_find_dev (bda);

  
 if (btm_cb.devcb.state != BTM_DEV_STATE_READY)
 {
        BTM_TRACE_EVENT ("Security Manager: connect request when device not ready");
        btsnd_hcic_reject_conn (bda, HCI_ERR_HOST_REJECT_DEVICE);
 return;
 }

  

  
 if (btm_cb.connect_only_paired)
 {
 if (!p_dev_rec || !(p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_AUTHED))
 {
            BTM_TRACE_EVENT ("Security Manager: connect request from non-paired device");
            btsnd_hcic_reject_conn (bda, HCI_ERR_HOST_REJECT_DEVICE);
 return;
 }
 }

#if BTM_ALLOW_CONN_IF_NONDISCOVER == FALSE
  
 if (btm_cb.btm_inq_vars.discoverable_mode == BTM_NON_DISCOVERABLE)
 {
 if (!p_dev_rec)
 {
            BTM_TRACE_EVENT ("Security Manager: connect request from not paired device");
            btsnd_hcic_reject_conn (bda, HCI_ERR_HOST_REJECT_DEVICE);
 return;
 }
 }
#endif

  
 if (btm_cb.p_conn_filter_cb)
 {
 if (!(* btm_cb.p_conn_filter_cb) (bda, dc))
 {
            BTM_TRACE_EVENT ("Security Manager: connect request did not pass filter");

  
            btsnd_hcic_reject_conn (bda, HCI_ERR_HOST_REJECT_DEVICE);
 return;
 }
 }

 if ((btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 &&(btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
 &&(!memcmp (btm_cb.pairing_bda, bda, BD_ADDR_LEN)))
 {
        BTM_TRACE_EVENT ("Security Manager: reject connect request from bonding device");

  
        btm_cb.pairing_flags |= BTM_PAIR_FLAGS_REJECTED_CONNECT;
        btsnd_hcic_reject_conn (bda, HCI_ERR_HOST_REJECT_DEVICE);
 return;
 }

  
  
    memcpy (btm_cb.connecting_bda, bda, BD_ADDR_LEN);
    memcpy (btm_cb.connecting_dc,  dc,  DEV_CLASS_LEN);

 if (l2c_link_hci_conn_req (bda))
 {
 if (!p_dev_rec)
 {
  
            p_dev_rec = btm_sec_alloc_dev (bda);
 }
 if (p_dev_rec)
 {
            p_dev_rec->sm4 |= BTM_SM4_CONN_PEND;
 }
 }
}
