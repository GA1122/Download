void smp_match_dhkey_checks(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t reason = SMP_DHKEY_CHK_FAIL;

  SMP_TRACE_DEBUG("%s", __func__);

 if (memcmp(p_data->key.p_data, p_cb->remote_dhkey_check, BT_OCTET16_LEN)) {
    SMP_TRACE_WARNING("dhkey chcks do no match");
    p_cb->failure = reason;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return;
 }

  SMP_TRACE_EVENT("dhkey chcks match");

  
 if (p_cb->peer_enc_size < p_cb->loc_enc_size)
    p_cb->loc_enc_size = p_cb->peer_enc_size;

 if (p_cb->role == HCI_ROLE_SLAVE) {
    smp_sm_event(p_cb, SMP_PAIR_DHKEY_CHCK_EVT, NULL);
 } else {
  
    p_cb->local_i_key = p_cb->peer_i_key;
    p_cb->local_r_key = p_cb->peer_r_key;
    smp_sm_event(p_cb, SMP_ENC_REQ_EVT, NULL);
 }
}
