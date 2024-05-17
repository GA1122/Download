void smp_key_distribution(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s role=%d (0-master) r_keys=0x%x i_keys=0x%x", __func__,
                  p_cb->role, p_cb->local_r_key, p_cb->local_i_key);

 if (p_cb->role == HCI_ROLE_SLAVE ||
 (!p_cb->local_r_key && p_cb->role == HCI_ROLE_MASTER)) {
    smp_key_pick_key(p_cb, p_data);
 }

 if (!p_cb->local_i_key && !p_cb->local_r_key) {
  
 if (smp_get_state() == SMP_STATE_BOND_PENDING) {
 if (p_cb->derive_lk) {
        smp_derive_link_key_from_long_term_key(p_cb, NULL);
        p_cb->derive_lk = false;
 }

 if (p_cb->total_tx_unacked == 0) {
  
 if (!alarm_is_scheduled(p_cb->delayed_auth_timer_ent)) {
          SMP_TRACE_DEBUG("%s delaying auth complete.", __func__);
          alarm_set_on_mloop(p_cb->delayed_auth_timer_ent,
                             SMP_DELAYED_AUTH_TIMEOUT_MS,
                             smp_delayed_auth_complete_timeout, NULL);
 }
 } else {
        p_cb->wait_for_authorization_complete = true;
 }
 }
 }
}
