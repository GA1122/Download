static void btm_restore_mode(void)
{
 if (btm_cb.security_mode_changed)
 {
        btm_cb.security_mode_changed = FALSE;
        BTM_TRACE_DEBUG("btm_restore_mode: Authen Enable -> %d", (btm_cb.security_mode == BTM_SEC_MODE_LINK));
        btsnd_hcic_write_auth_enable ((UINT8)(btm_cb.security_mode == BTM_SEC_MODE_LINK));
 }

 if (btm_cb.pin_type_changed)
 {
        btm_cb.pin_type_changed = FALSE;
        btsnd_hcic_write_pin_type (btm_cb.cfg.pin_type);
 }
}
