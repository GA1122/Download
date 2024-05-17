static tBTM_STATUS btm_sec_dd_create_conn (tBTM_SEC_DEV_REC *p_dev_rec)
{
    tL2C_LCB         *p_lcb;

  
 if ((p_lcb = l2cu_allocate_lcb (p_dev_rec->bd_addr, TRUE, BT_TRANSPORT_BR_EDR)) == NULL)
 {
        BTM_TRACE_WARNING ("Security Manager: failed allocate LCB [%02x%02x%02x%02x%02x%02x]",
                            p_dev_rec->bd_addr[0], p_dev_rec->bd_addr[1], p_dev_rec->bd_addr[2],
                            p_dev_rec->bd_addr[3], p_dev_rec->bd_addr[4], p_dev_rec->bd_addr[5]);

 return(BTM_NO_RESOURCES);
 }

  
    btm_cb.pairing_flags |= BTM_PAIR_FLAGS_DISC_WHEN_DONE;

 if (l2cu_create_conn(p_lcb, BT_TRANSPORT_BR_EDR) == FALSE)
 {
        BTM_TRACE_WARNING ("Security Manager: failed create  [%02x%02x%02x%02x%02x%02x]",
                            p_dev_rec->bd_addr[0], p_dev_rec->bd_addr[1], p_dev_rec->bd_addr[2],
                            p_dev_rec->bd_addr[3], p_dev_rec->bd_addr[4], p_dev_rec->bd_addr[5]);

        l2cu_release_lcb(p_lcb);
 return(BTM_NO_RESOURCES);
 }

#if (defined(BTM_BUSY_LEVEL_CHANGE_INCLUDED) && BTM_BUSY_LEVEL_CHANGE_INCLUDED == TRUE)
    btm_acl_update_busy_level (BTM_BLI_PAGE_EVT);
#endif

    BTM_TRACE_DEBUG ("Security Manager: btm_sec_dd_create_conn [%02x%02x%02x%02x%02x%02x]",
                      p_dev_rec->bd_addr[0], p_dev_rec->bd_addr[1], p_dev_rec->bd_addr[2],
                      p_dev_rec->bd_addr[3], p_dev_rec->bd_addr[4], p_dev_rec->bd_addr[5]);

    btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_PIN_REQ);

 return(BTM_CMD_STARTED);
}
