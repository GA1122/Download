static void btm_sec_change_pairing_state (tBTM_PAIRING_STATE new_state)
{
    tBTM_PAIRING_STATE  old_state = btm_cb.pairing_state;

    BTM_TRACE_EVENT ("btm_sec_change_pairing_state  Old: %s",  btm_pair_state_descr(btm_cb.pairing_state));
    BTM_TRACE_EVENT ("btm_sec_change_pairing_state  New: %s pairing_flags:0x%x",btm_pair_state_descr(new_state), btm_cb.pairing_flags);

    btm_cb.pairing_state = new_state;

 if (new_state == BTM_PAIR_STATE_IDLE)
 {
        btu_stop_timer (&btm_cb.pairing_tle);

        btm_cb.pairing_flags = 0;
        btm_cb.pin_code_len  = 0;

  
        l2cu_update_lcb_4_bonding (btm_cb.pairing_bda, FALSE);

        btm_restore_mode();
        btm_sec_check_pending_reqs();
        btm_inq_clear_ssp();

        memset (btm_cb.pairing_bda, 0xFF, BD_ADDR_LEN);
 }
 else
 {
  
 if (old_state == BTM_PAIR_STATE_IDLE)
            l2cu_update_lcb_4_bonding (btm_cb.pairing_bda, TRUE);

        btm_cb.pairing_tle.param = (TIMER_PARAM_TYPE)btm_sec_pairing_timeout;

        btu_start_timer (&btm_cb.pairing_tle, BTU_TTYPE_USER_FUNC, BTM_SEC_TIMEOUT_VALUE);
 }
}
