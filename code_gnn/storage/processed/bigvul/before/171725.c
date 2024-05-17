static void btif_jni_disassociate(UNUSED_ATTR uint16_t event, UNUSED_ATTR char *p_param) {
  BTIF_TRACE_DEBUG("%s Disassociating thread from JVM", __func__);
  HAL_CBACK(bt_hal_cbacks, thread_evt_cb, DISASSOCIATE_JVM);
  bt_hal_cbacks = NULL;
  future_ready(stack_manager_get_hack_future(), FUTURE_SUCCESS);
}
