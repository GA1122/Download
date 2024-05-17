void smp_br_process_link_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tSMP_STATUS status = SMP_PAIR_FAIL_UNKNOWN;

  SMP_TRACE_DEBUG("%s", __func__);
 if (!smp_calculate_long_term_key_from_link_key(p_cb)) {
    SMP_TRACE_ERROR("%s: failed", __func__);
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = status;
    smp_sm_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &smp_int_data);
 return;
 }

  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(p_cb->pairing_bda);
 if (p_dev_rec) {
    SMP_TRACE_DEBUG("%s: dev_type = %d ", __func__, p_dev_rec->device_type);
    p_dev_rec->device_type |= BT_DEVICE_TYPE_BLE;
 } else {
    SMP_TRACE_ERROR("%s failed to find Security Record", __func__);
 }

  SMP_TRACE_DEBUG("%s: LTK derivation from LK successfully completed",
                  __func__);
  smp_save_secure_connections_long_term_key(p_cb);
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_ENC, false);
  smp_br_select_next_key(p_cb, NULL);
}
