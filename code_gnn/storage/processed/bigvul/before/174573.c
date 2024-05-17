void BTM_SetSecurityMode (UINT8 security_mode)
{
    UINT8   old_mode = btm_cb.security_mode;

    UINT8   sp_mode = HCI_SP_MODE_ENABLED;
    UINT8   sp_debug_mode = HCI_SPD_MODE_DISABLED;

 switch (security_mode)
 {
#if (BTM_PRE_LISBON_INCLUDED == TRUE)
 case BTM_SEC_MODE_NONE:
 case BTM_SEC_MODE_SERVICE:
 case BTM_SEC_MODE_LINK:
 break;
#endif

 case BTM_SEC_MODE_SP_DEBUG:
            sp_debug_mode = HCI_SPD_MODE_ENABLED;
 break;
 case BTM_SEC_MODE_SP:
  
 break;
 default:
            BTM_TRACE_ERROR ("BTM_SetSecurityMode: unknown mode:%d", security_mode);
 return;
 }
    btm_cb.security_mode = security_mode;

 if (HCI_SIMPLE_PAIRING_SUPPORTED(btm_cb.devcb.local_lmp_features[HCI_EXT_FEATURES_PAGE_0]))
 {
  
        btm_cb.security_mode = BTM_SEC_MODE_SP;
        BTM_TRACE_DEBUG("BTM_SetSecurityMode: SP:%d, debug:%d", sp_mode, sp_debug_mode);
        btsnd_hcic_write_simple_pairing_mode(sp_mode);
        btsnd_hcic_write_simp_pair_debug_mode(sp_debug_mode);
 return;
 }

  
#if (BTM_PRE_LISBON_INCLUDED == TRUE)
  
  
 if ((old_mode == BTM_SEC_MODE_LINK)
 && (       security_mode != BTM_SEC_MODE_LINK))
 {
        BTM_TRACE_DEBUG("BTM_SetSecurityMode: Authen Enable -> FALSE");
        btsnd_hcic_write_auth_enable (FALSE);
        btsnd_hcic_write_encr_mode (HCI_ENCRYPT_MODE_DISABLED);
 }

  
  
 if ((old_mode != BTM_SEC_MODE_LINK)
 && (       security_mode == BTM_SEC_MODE_LINK))
 {
        BTM_TRACE_DEBUG("BTM_SetSecurityMode: Authen Enable -> TRUE");
        btsnd_hcic_write_auth_enable (TRUE);
        btsnd_hcic_write_encr_mode (HCI_ENCRYPT_MODE_POINT_TO_POINT);
 }
#endif  
}
