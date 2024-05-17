void smp_both_have_public_keys(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

  
  smp_compute_dhkey(p_cb);

  
 if (p_cb->role == HCI_ROLE_SLAVE) smp_send_pair_public_key(p_cb, NULL);

  smp_sm_event(p_cb, SMP_SC_DHKEY_CMPLT_EVT, NULL);
}
