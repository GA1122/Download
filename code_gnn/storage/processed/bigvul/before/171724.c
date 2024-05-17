static void btif_jni_associate(UNUSED_ATTR uint16_t event, UNUSED_ATTR char *p_param) {
  BTIF_TRACE_DEBUG("%s Associating thread to JVM", __func__);
  HAL_CBACK(bt_hal_cbacks, thread_evt_cb, ASSOCIATE_JVM);
}
