BOOLEAN btm_sec_create_conn (BD_ADDR bda, UINT16 packet_types,
                             UINT8 page_scan_rep_mode, UINT8 page_scan_mode,
                             UINT16 clock_offset, UINT8 allow_switch)
{
    tBTM_SEC_DEV_REC *p_dev_rec = btm_find_or_alloc_dev (bda);

    memcpy (btm_cb.connecting_bda, p_dev_rec->bd_addr,   BD_ADDR_LEN);
    memcpy (btm_cb.connecting_dc,  p_dev_rec->dev_class, DEV_CLASS_LEN);

    btm_cb.acl_disc_reason = 0xff ;

    p_dev_rec->sec_state   = BTM_SEC_STATE_IDLE;
    p_dev_rec->role_master = TRUE;

  
 if (BTM_GetNumScoLinks() != 0)
        allow_switch = HCI_CR_CONN_NOT_ALLOW_SWITCH;

 return(btsnd_hcic_create_conn (bda, packet_types, page_scan_rep_mode,
                                   page_scan_mode, clock_offset, allow_switch));
}
