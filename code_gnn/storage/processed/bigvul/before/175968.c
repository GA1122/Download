void smp_wait_for_both_public_keys(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

 if ((p_cb->flags & SMP_PAIR_FLAG_HAVE_PEER_PUBL_KEY) &&
 (p_cb->flags & SMP_PAIR_FLAG_HAVE_LOCAL_PUBL_KEY)) {
 if ((p_cb->role == HCI_ROLE_SLAVE) &&
 ((p_cb->req_oob_type == SMP_OOB_LOCAL) ||
 (p_cb->req_oob_type == SMP_OOB_BOTH))) {
      smp_set_state(SMP_STATE_PUBLIC_KEY_EXCH);
 }
    smp_sm_event(p_cb, SMP_BOTH_PUBL_KEYS_RCVD_EVT, NULL);
 }
}
