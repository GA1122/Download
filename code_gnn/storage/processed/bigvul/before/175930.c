bool smp_proc_ltk_request(const RawAddress& bda) {
  SMP_TRACE_DEBUG("%s state = %d", __func__, smp_cb.state);
 bool match = false;

 if (bda == smp_cb.pairing_bda) {
    match = true;
 } else {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bda);
 if (p_dev_rec != NULL && p_dev_rec->ble.pseudo_addr == smp_cb.pairing_bda &&
        p_dev_rec->ble.pseudo_addr != RawAddress::kEmpty) {
      match = true;
 }
 }

 if (match && smp_cb.state == SMP_STATE_ENCRYPTION_PENDING) {
    smp_sm_event(&smp_cb, SMP_ENC_REQ_EVT, NULL);
 return true;
 }

 return false;
}
