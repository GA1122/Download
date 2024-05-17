tBTM_STATUS btm_sec_disconnect (UINT16 handle, UINT8 reason)
{
    tBTM_SEC_DEV_REC  *p_dev_rec = btm_find_dev_by_handle (handle);

  
 if (!p_dev_rec)
 {
        btsnd_hcic_disconnect (handle, reason);
 return(BTM_SUCCESS);
 }

  
 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (memcmp (btm_cb.pairing_bda, p_dev_rec->bd_addr, BD_ADDR_LEN) == 0)
 && (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD) )
 {
  
        btm_cb.pairing_flags |= BTM_PAIR_FLAGS_DISC_WHEN_DONE;
 return(BTM_BUSY);
 }

 return(btm_sec_send_hci_disconnect(p_dev_rec, reason, handle));
}
