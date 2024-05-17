void btm_sec_link_key_notification (UINT8 *p_bda, UINT8 *p_link_key, UINT8 key_type)
{
    tBTM_SEC_DEV_REC *p_dev_rec = btm_find_or_alloc_dev (p_bda);
    BOOLEAN          we_are_bonding = FALSE;

    BTM_TRACE_EVENT ("btm_sec_link_key_notification()  BDA:%04x%08x, TYPE: %d",
 (p_bda[0]<<8)+p_bda[1], (p_bda[2]<<24)+(p_bda[3]<<16)+(p_bda[4]<<8)+p_bda[5],
                      key_type);

  
    btm_restore_mode();

  
 if (btm_cb.devcb.local_version.hci_version < HCI_VERSION_1_1)
        p_dev_rec->link_key_type = BTM_LKEY_TYPE_IGNORE;
 if (key_type != BTM_LKEY_TYPE_CHANGED_COMB)
        p_dev_rec->link_key_type = key_type;

    p_dev_rec->sec_flags |= BTM_SEC_LINK_KEY_KNOWN;
#if (BLE_INCLUDED == TRUE)
  
    p_dev_rec->enc_key_size = 16;
#endif
    memcpy (p_dev_rec->link_key, p_link_key, LINK_KEY_LEN);

 if ( (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 && (memcmp (btm_cb.pairing_bda, p_bda, BD_ADDR_LEN) == 0) )
 {
 if (btm_cb.pairing_flags & BTM_PAIR_FLAGS_WE_STARTED_DD)
            we_are_bonding = TRUE;
 else
            btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
 }

  
  
 if ((!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN)
 && ((p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK) != BTM_COD_MAJOR_PERIPHERAL)) )
 {
        BTM_TRACE_EVENT ("btm_sec_link_key_notification()  Delayed BDA: %08x%04x Type:%d",
 (p_bda[0]<<24) + (p_bda[1]<<16) + (p_bda[2]<<8) + p_bda[3], (p_bda[4] << 8) + p_bda[5], key_type);

        p_dev_rec->link_key_not_sent = TRUE;

  
 if (we_are_bonding)
 {
 if (!(btsnd_hcic_rmt_name_req (p_bda, HCI_PAGE_SCAN_REP_MODE_R1, HCI_MANDATARY_PAGE_SCAN_MODE, 0)))
                btm_inq_rmt_name_failed();
 }

        BTM_TRACE_EVENT ("rmt_io_caps:%d, sec_flags:x%x, dev_class[1]:x%02x", p_dev_rec->rmt_io_caps, p_dev_rec->sec_flags, p_dev_rec->dev_class[1])
 return;
 }

  
  
  
 if (!(p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE))
 {
 if (btm_cb.api.p_auth_complete_callback)
 (*btm_cb.api.p_auth_complete_callback) (p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                                    p_dev_rec->sec_bd_name, HCI_SUCCESS);
 }

  
#ifdef BRCM_NONE_BTE
 if (p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_AUTHED)
#endif
 {
 if (btm_cb.api.p_link_key_callback)
 {
 (*btm_cb.api.p_link_key_callback) (p_bda, p_dev_rec->dev_class,  p_dev_rec->sec_bd_name,
                                               p_link_key, p_dev_rec->link_key_type);
 }
 }
}
