void smp_set_local_oob_random_commitment(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  memcpy(p_cb->sc_oob_data.loc_oob_data.randomizer, p_cb->rand, BT_OCTET16_LEN);

  smp_calculate_f4(p_cb->sc_oob_data.loc_oob_data.publ_key_used.x,
                   p_cb->sc_oob_data.loc_oob_data.publ_key_used.x,
                   p_cb->sc_oob_data.loc_oob_data.randomizer, 0,
                   p_cb->sc_oob_data.loc_oob_data.commitment);

#if (SMP_DEBUG == TRUE)
 uint8_t* p_print = NULL;
  SMP_TRACE_DEBUG("local SC OOB data set:");
  p_print = (uint8_t*)&p_cb->sc_oob_data.loc_oob_data.addr_sent_to;
  smp_debug_print_nbyte_little_endian(p_print, "addr_sent_to",
 sizeof(tBLE_BD_ADDR));
  p_print = (uint8_t*)&p_cb->sc_oob_data.loc_oob_data.private_key_used;
  smp_debug_print_nbyte_little_endian(p_print, "private_key_used",
                                      BT_OCTET32_LEN);
  p_print = (uint8_t*)&p_cb->sc_oob_data.loc_oob_data.publ_key_used.x;
  smp_debug_print_nbyte_little_endian(p_print, "publ_key_used.x",
                                      BT_OCTET32_LEN);
  p_print = (uint8_t*)&p_cb->sc_oob_data.loc_oob_data.publ_key_used.y;
  smp_debug_print_nbyte_little_endian(p_print, "publ_key_used.y",
                                      BT_OCTET32_LEN);
  p_print = (uint8_t*)&p_cb->sc_oob_data.loc_oob_data.randomizer;
  smp_debug_print_nbyte_little_endian(p_print, "randomizer", BT_OCTET16_LEN);
  p_print = (uint8_t*)&p_cb->sc_oob_data.loc_oob_data.commitment;
  smp_debug_print_nbyte_little_endian(p_print, "commitment", BT_OCTET16_LEN);
  SMP_TRACE_DEBUG("");
#endif

  
  p_cb->cb_evt = SMP_SC_LOC_OOB_DATA_UP_EVT;
  smp_send_app_cback(p_cb, NULL);

  smp_cb_cleanup(p_cb);
}
