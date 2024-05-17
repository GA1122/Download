tBTM_STATUS BTM_SecBondCancel (BD_ADDR bd_addr)
{
    tBTM_SEC_DEV_REC *p_dev_rec;

    BTM_TRACE_API ("BTM_SecBondCancel()  State: %s flags:0x%x",
                    btm_pair_state_descr (btm_cb.pairing_state), btm_cb.pairing_flags);

 if (((p_dev_rec = btm_find_dev (bd_addr)) == NULL)
 || (memcmp (btm_cb.pairing_bda, bd_addr, BD_ADDR_LEN) != 0) )
 return BTM_UNKNOWN_ADDR;

#if SMP_INCLUDED == TRUE
 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_LE_ACTIVE)
 {
 if (p_dev_rec->sec_state == BTM_SEC_STATE_AUTHENTICATING)
 {
            BTM_TRACE_DEBUG ("Cancel LE pairing");
 if (SMP_PairCancel(bd_addr))
 {
 return BTM_CMD_STARTED;
 }
 }
 return BTM_WRONG_MODE;
 }

#endif
    BTM_TRACE_DEBUG ("hci_handle:0x%x sec_state:%d", p_dev_rec->hci_handle, p_dev_rec->sec_state );
 if (BTM_PAIR_STATE_WAIT_LOCAL_PIN == btm_cb.pairing_state &&
        BTM_PAIR_FLAGS_WE_STARTED_DD & btm_cb.pairing_flags)
 {
  
        btm_sec_bond_cancel_complete();
 return BTM_SUCCESS;
 }

  
 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD))
 {
  
 if (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE)
 {
  
 if (p_dev_rec->sec_state == BTM_SEC_STATE_DISCONNECTING)
 return(BTM_CMD_STARTED);

  
 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE)
 return btm_sec_send_hci_disconnect(p_dev_rec, HCI_ERR_PEER_USER, p_dev_rec->hci_handle);
 else
                l2cu_update_lcb_4_bonding(bd_addr, FALSE);

 return BTM_NOT_AUTHORIZED;
 }
 else  
 {
  
 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_DISC_WHEN_DONE)
 {
 if (btsnd_hcic_create_conn_cancel(bd_addr))
 return BTM_CMD_STARTED;

 return BTM_NO_RESOURCES;
 }
 if (btm_cb.pairing_state == BTM_PAIR_STATE_GET_REM_NAME)
 {
                BTM_CancelRemoteDeviceName();
                btm_cb.pairing_flags |= BTM_PAIR_FLAGS_WE_CANCEL_DD;
 return BTM_CMD_STARTED;
 }
 return BTM_NOT_AUTHORIZED;
 }
 }

 return BTM_WRONG_MODE;
}
