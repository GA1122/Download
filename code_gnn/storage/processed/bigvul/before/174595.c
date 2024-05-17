static BOOLEAN btm_sec_check_prefetch_pin (tBTM_SEC_DEV_REC  *p_dev_rec)
{
    UINT8 major = (UINT8)(p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK);
    UINT8 minor = (UINT8)(p_dev_rec->dev_class[2] & BTM_COD_MINOR_CLASS_MASK);
    BOOLEAN rv = FALSE;

 if ((major == BTM_COD_MAJOR_AUDIO)
 && ((minor == BTM_COD_MINOR_CONFM_HANDSFREE) || (minor == BTM_COD_MINOR_CAR_AUDIO)) )
 {
        BTM_TRACE_EVENT ("btm_sec_check_prefetch_pin: Skipping pre-fetch PIN for carkit COD Major: 0x%02x Minor: 0x%02x", major, minor);

 if (btm_cb.security_mode_changed == FALSE)
 {
            btm_cb.security_mode_changed = TRUE;
#ifdef APPL_AUTH_WRITE_EXCEPTION
 if(!(APPL_AUTH_WRITE_EXCEPTION)(p_dev_rec->bd_addr))
#endif
                btsnd_hcic_write_auth_enable (TRUE);
 }
 }
 else
 {
        btm_sec_change_pairing_state (BTM_PAIR_STATE_WAIT_LOCAL_PIN);

  
 if (btm_cb.pin_code_len)
 {
            BTM_PINCodeReply (p_dev_rec->bd_addr, BTM_SUCCESS, btm_cb.pin_code_len, btm_cb.pin_code, p_dev_rec->trusted_mask);
 }
 else
 {
  
 if (btm_cb.api.p_pin_callback && ((btm_cb.pairing_flags & BTM_PAIR_FLAGS_PIN_REQD) == 0))
 {
                BTM_TRACE_DEBUG("btm_sec_check_prefetch_pin: PIN code callback called");
 if (btm_bda_to_acl(p_dev_rec->bd_addr, BT_TRANSPORT_BR_EDR) == NULL)
                btm_cb.pairing_flags |= BTM_PAIR_FLAGS_PIN_REQD;
 (btm_cb.api.p_pin_callback) (p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name);
 }
 }

        rv = TRUE;
 }

 return rv;
}
