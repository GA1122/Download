void smp_send_pair_req(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(p_cb->pairing_bda);
  SMP_TRACE_DEBUG("%s", __func__);

  
 if (p_dev_rec) btm_sec_clear_ble_keys(p_dev_rec);
  
  smp_send_cmd(SMP_OPCODE_PAIRING_REQ, p_cb);
}
