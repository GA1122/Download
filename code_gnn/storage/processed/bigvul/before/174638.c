void btm_simple_pair_complete (UINT8 *p)
{
    tBTM_SP_COMPLT  evt_data;
    tBTM_SEC_DEV_REC *p_dev_rec;
    UINT8           status;
    BOOLEAN         disc = FALSE;

    status = *p++;
    STREAM_TO_BDADDR (evt_data.bd_addr, p);

 if ((p_dev_rec = btm_find_dev (evt_data.bd_addr)) == NULL)
 {
        BTM_TRACE_ERROR ("btm_simple_pair_complete() with unknown BDA: %08x%04x",
 (evt_data.bd_addr[0]<<24) + (evt_data.bd_addr[1]<<16) + (evt_data.bd_addr[2]<<8) + evt_data.bd_addr[3],
 (evt_data.bd_addr[4] << 8) + evt_data.bd_addr[5]);
 return;
 }

    BTM_TRACE_EVENT ("btm_simple_pair_complete()  Pair State: %s  Status:%d  sec_state: %u",
                      btm_pair_state_descr(btm_cb.pairing_state),  status, p_dev_rec->sec_state);

    evt_data.status = BTM_ERR_PROCESSING;
 if (status == HCI_SUCCESS)
 {
        evt_data.status = BTM_SUCCESS;
        p_dev_rec->sec_flags |= BTM_SEC_AUTHENTICATED;
 }
 else
 {
 if (status == HCI_ERR_PAIRING_NOT_ALLOWED)
 {
  
            btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_DISCONNECT);

  
            btu_start_timer (&btm_cb.pairing_tle, BTU_TTYPE_USER_FUNC, BT_1SEC_TIMEOUT);
 }
 else if (memcmp (btm_cb.pairing_bda, evt_data.bd_addr, BD_ADDR_LEN) == 0)
 {
  
            btu_stop_timer (&btm_cb.pairing_tle);

 if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING)
 {
  
                disc = TRUE;
 }
 }
 else
            disc = TRUE;
 }

  
    memcpy (evt_data.bd_addr, p_dev_rec->bd_addr, BD_ADDR_LEN);
    memcpy (evt_data.dev_class, p_dev_rec->dev_class, DEV_CLASS_LEN);

 if (btm_cb.api.p_sp_callback)
 (*btm_cb.api.p_sp_callback) (BTM_SP_COMPLT_EVT, (tBTM_SP_EVT_DATA *)&evt_data);

 if (disc)
 {
  
  
 if ((status != HCI_ERR_PEER_USER) && (status != HCI_ERR_CONN_CAUSE_LOCAL_HOST))
 {
            btm_sec_send_hci_disconnect (p_dev_rec, HCI_ERR_AUTH_FAILURE, p_dev_rec->hci_handle);
 }
 }
}
