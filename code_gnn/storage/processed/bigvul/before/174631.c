static tBTM_STATUS btm_sec_send_hci_disconnect (tBTM_SEC_DEV_REC *p_dev_rec, UINT8 reason, UINT16 conn_handle)
{
    UINT8       old_state = p_dev_rec->sec_state;
    tBTM_STATUS status = BTM_CMD_STARTED;

    BTM_TRACE_EVENT ("btm_sec_send_hci_disconnect:  handle:0x%x, reason=0x%x",
                      conn_handle, reason);

  
 if (BTM_SEC_STATE_DISCONNECTING != old_state)
 {
        p_dev_rec->sec_state = BTM_SEC_STATE_DISCONNECTING;

#if BTM_DISC_DURING_RS == TRUE
  
 if (p_dev_rec->rs_disc_pending == BTM_SEC_RS_PENDING &&
             p_dev_rec->hci_handle == conn_handle)

 {
                 BTM_TRACE_DEBUG("RS in progress - Set DISC Pending flag in btm_sec_send_hci_disconnect to delay disconnect");
                 p_dev_rec->rs_disc_pending = BTM_SEC_DISC_PENDING;
                 status = BTM_SUCCESS;
 }
 else
#endif
  
 if (!btsnd_hcic_disconnect (conn_handle, reason))
 {
  
            p_dev_rec->sec_state = old_state;
            status = BTM_NO_RESOURCES;
 }
 }
 return (status);
}
