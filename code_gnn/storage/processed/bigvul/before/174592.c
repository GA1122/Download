tBTM_STATUS btm_sec_bond_by_transport (BD_ADDR bd_addr, tBT_TRANSPORT transport,
                                       UINT8 pin_len, UINT8 *p_pin, UINT32 trusted_mask[])
{
    tBTM_SEC_DEV_REC *p_dev_rec;
    tBTM_STATUS      status;
    UINT8            *p_features;
    UINT8            ii;
    tACL_CONN        *p= btm_bda_to_acl(bd_addr, transport);
    BTM_TRACE_API ("btm_sec_bond_by_transport BDA: %02x:%02x:%02x:%02x:%02x:%02x",
                    bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

    BTM_TRACE_DEBUG("btm_sec_bond_by_transport: Transport used %d" , transport);


  
 if (btm_cb.pairing_state != BTM_PAIR_STATE_IDLE)
 {
        BTM_TRACE_ERROR ("BTM_SecBond: already busy in state: %s", btm_pair_state_descr(btm_cb.pairing_state));
 return(BTM_WRONG_MODE);
 }

 if ((p_dev_rec = btm_find_or_alloc_dev (bd_addr)) == NULL)
 {
 return(BTM_NO_RESOURCES);
 }

    BTM_TRACE_DEBUG ("before update sec_flags=0x%x", p_dev_rec->sec_flags);

  
 if ( ((p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE) && transport == BT_TRANSPORT_BR_EDR
 && (p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED))
#if (BLE_INCLUDED == TRUE)
 ||((p_dev_rec->ble_hci_handle != BTM_SEC_INVALID_HANDLE) && transport == BT_TRANSPORT_LE
 && (p_dev_rec->sec_flags & BTM_SEC_LE_AUTHENTICATED))
#endif

 )
 {
        BTM_TRACE_WARNING("BTM_SecBond -> Already Paired");
 return(BTM_SUCCESS);
 }

  
 if ((BTM_DeleteStoredLinkKey (bd_addr, NULL)) != BTM_SUCCESS)
 return(BTM_NO_RESOURCES);

  
 if (p_pin && (pin_len <= PIN_CODE_LEN) && (pin_len != 0))
 {
        btm_cb.pin_code_len = pin_len;
        memcpy (btm_cb.pin_code, p_pin, PIN_CODE_LEN);
 }

    memcpy (btm_cb.pairing_bda, bd_addr, BD_ADDR_LEN);

    btm_cb.pairing_flags = BTM_PAIR_FLAGS_WE_STARTED_DD;

    p_dev_rec->security_required = BTM_SEC_OUT_AUTHENTICATE;
    p_dev_rec->is_originator     = TRUE;
 if (trusted_mask)
        BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
 if (transport == BT_TRANSPORT_LE)
 {
        p_dev_rec->sec_flags &= ~ BTM_SEC_LE_MASK;

 if (SMP_Pair(bd_addr) == SMP_STARTED)
 {
            btm_cb.pairing_flags |= BTM_PAIR_FLAGS_LE_ACTIVE;
            p_dev_rec->sec_state = BTM_SEC_STATE_AUTHENTICATING;
            btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_AUTH_COMPLETE);
 return BTM_CMD_STARTED;
 }

        btm_cb.pairing_flags = 0;
 return(BTM_NO_RESOURCES);
 }
#endif

    p_dev_rec->sec_flags &= ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED
 | BTM_SEC_ROLE_SWITCHED  | BTM_SEC_LINK_KEY_AUTHED);


    BTM_TRACE_DEBUG ("after update sec_flags=0x%x", p_dev_rec->sec_flags);
 if (!HCI_SIMPLE_PAIRING_SUPPORTED(btm_cb.devcb.local_lmp_features[HCI_EXT_FEATURES_PAGE_0]))
 {
  
  
  
 if (((p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK) == BTM_COD_MAJOR_PERIPHERAL)
 && (p_dev_rec->dev_class[2] & BTM_COD_MINOR_KEYBOARD)
 && (btm_cb.cfg.pin_type != HCI_PIN_TYPE_FIXED))
 {
            btm_cb.pin_type_changed = TRUE;
            btsnd_hcic_write_pin_type (HCI_PIN_TYPE_FIXED);
 }
 }

 for (ii = 0; ii <= HCI_EXT_FEATURES_PAGE_MAX; ii++)
 {
        p_features = p_dev_rec->features[ii];
        BTM_TRACE_EVENT("  remote_features page[%1d] = %02x-%02x-%02x-%02x",
                         ii, p_features[0], p_features[1], p_features[2], p_features[3]);
        BTM_TRACE_EVENT("                              %02x-%02x-%02x-%02x",
                             p_features[4], p_features[5], p_features[6], p_features[7]);
 }

    BTM_TRACE_EVENT ("BTM_SecBond: Remote sm4: 0x%x  HCI Handle: 0x%04x", p_dev_rec->sm4, p_dev_rec->hci_handle);

#if BTM_SEC_FORCE_RNR_FOR_DBOND == TRUE
    p_dev_rec->sec_flags &= ~BTM_SEC_NAME_KNOWN;
#endif

  
 if (p && p->hci_handle != BTM_SEC_INVALID_HANDLE)
 {
 if (!btm_sec_start_authentication (p_dev_rec))
 return(BTM_NO_RESOURCES);

        btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_PIN_REQ);

  
        l2cu_update_lcb_4_bonding (bd_addr, TRUE);
 return(BTM_CMD_STARTED);
 }

    BTM_TRACE_DEBUG ("sec mode: %d sm4:x%x", btm_cb.security_mode, p_dev_rec->sm4);
 if (!HCI_SIMPLE_PAIRING_SUPPORTED(btm_cb.devcb.local_lmp_features[HCI_EXT_FEATURES_PAGE_0])
 || (p_dev_rec->sm4 == BTM_SM4_KNOWN))
 {
 if ( btm_sec_check_prefetch_pin (p_dev_rec) )
 return(BTM_CMD_STARTED);
 }
 if (BTM_SEC_MODE_SP == btm_cb.security_mode && BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4))
 {
  
 if ((p_dev_rec->sm4 & BTM_SM4_CONN_PEND) == 0)
 {
  
            btm_sec_change_pairing_state (BTM_PAIR_STATE_GET_REM_NAME);
            BTM_ReadRemoteDeviceName(bd_addr, NULL, BT_TRANSPORT_BR_EDR);
 }
 else
 {
  
            btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_PIN_REQ);
 }
        BTM_TRACE_DEBUG ("State:%s sm4: 0x%x sec_state:%d",
            btm_pair_state_descr (btm_cb.pairing_state), p_dev_rec->sm4, p_dev_rec->sec_state);
 return BTM_CMD_STARTED;
 }

  
    status = btm_sec_dd_create_conn(p_dev_rec);

 if (status != BTM_CMD_STARTED)
 {
        btm_sec_change_pairing_state (BTM_PAIR_STATE_IDLE);
 }

 return status;
}
