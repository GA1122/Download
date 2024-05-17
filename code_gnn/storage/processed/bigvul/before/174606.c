void btm_sec_dev_reset (void)
{
#if (BTM_PRE_LISBON_INCLUDED == TRUE)
 if (btm_cb.security_mode == BTM_SEC_MODE_LINK)
 {
        btsnd_hcic_write_auth_enable (TRUE);
        btsnd_hcic_write_encr_mode (HCI_ENCRYPT_MODE_POINT_TO_POINT);
 }
#endif
#if (BTM_PRE_LISBON_INCLUDED == TRUE)
 else
#endif
  
 if (HCI_SIMPLE_PAIRING_SUPPORTED(btm_cb.devcb.local_lmp_features[HCI_EXT_FEATURES_PAGE_0]))
 {
        btsnd_hcic_write_simple_pairing_mode(HCI_SP_MODE_ENABLED);
#if BLE_INCLUDED == TRUE
        btsnd_hcic_set_event_mask(LOCAL_BR_EDR_CONTROLLER_ID,
 (UINT8 *)HCI_DUMO_EVENT_MASK_EXT);

        btsnd_hcic_ble_set_evt_mask((UINT8 *)HCI_BLE_EVENT_MASK_DEF);

#else
        btsnd_hcic_set_event_mask(LOCAL_BR_EDR_CONTROLLER_ID,
 (UINT8 *)HCI_LISBON_EVENT_MASK_EXT);
#endif
  
        btm_cb.devcb.loc_io_caps = BTM_LOCAL_IO_CAPS;
  
#if (RFCOMM_INCLUDED == TRUE)
        BTM_SetSecurityLevel(FALSE, "RFC_MUX", BTM_SEC_SERVICE_RFC_MUX,
                             BTM_SEC_NONE, BT_PSM_RFCOMM, BTM_SEC_PROTO_RFCOMM, 0);
#endif
 }
 else
 {
        btm_cb.security_mode = BTM_SEC_MODE_SERVICE;
 }

    BTM_TRACE_DEBUG ("btm_sec_dev_reset sec mode: %d", btm_cb.security_mode);
}
