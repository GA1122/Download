void smp_process_pairing_public_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = (uint8_t*)p_data;
 uint8_t reason = SMP_INVALID_PARAMETERS;

  SMP_TRACE_DEBUG("%s", __func__);

 if (smp_command_has_invalid_parameters(p_cb)) {
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return;
 }

  STREAM_TO_ARRAY(p_cb->peer_publ_key.x, p, BT_OCTET32_LEN);
  STREAM_TO_ARRAY(p_cb->peer_publ_key.y, p, BT_OCTET32_LEN);

 Point pt;
  memcpy(pt.x, p_cb->peer_publ_key.x, BT_OCTET32_LEN);
  memcpy(pt.y, p_cb->peer_publ_key.y, BT_OCTET32_LEN);

 if (!ECC_ValidatePoint(pt)) {
    android_errorWriteLog(0x534e4554, "72377774");
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return;
 }

  p_cb->flags |= SMP_PAIR_FLAG_HAVE_PEER_PUBL_KEY;

  smp_wait_for_both_public_keys(p_cb, NULL);
}
