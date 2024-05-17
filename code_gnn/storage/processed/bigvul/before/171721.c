void btif_init_ok(UNUSED_ATTR uint16_t event, UNUSED_ATTR char *p_param) {
  BTIF_TRACE_DEBUG("btif_task: received trigger stack init event");
#if (BLE_INCLUDED == TRUE)
  btif_dm_load_ble_local_keys();
#endif
  BTA_EnableBluetooth(bte_dm_evt);
}
