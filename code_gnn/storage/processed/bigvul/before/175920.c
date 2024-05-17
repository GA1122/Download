void smp_link_encrypted(const RawAddress& bda, uint8_t encr_enable) {
  tSMP_CB* p_cb = &smp_cb;

  SMP_TRACE_DEBUG("%s: encr_enable=%d", __func__, encr_enable);

 if (smp_cb.pairing_bda == bda) {
  
 if (p_cb->loc_enc_size != 0 && encr_enable) {
  
      btm_ble_update_sec_key_size(bda, p_cb->loc_enc_size);
 }

    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = encr_enable;
    smp_sm_event(&smp_cb, SMP_ENCRYPTED_EVT, &smp_int_data);
 }
}
