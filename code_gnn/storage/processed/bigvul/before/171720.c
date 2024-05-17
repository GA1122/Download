void btif_init_fail(UNUSED_ATTR uint16_t event, UNUSED_ATTR char *p_param) {
  BTIF_TRACE_DEBUG("btif_task: hardware init failed");
  bte_main_disable();
  btif_queue_release();
  bte_main_shutdown();
  btif_dut_mode = 0;

  future_ready(stack_manager_get_hack_future(), FUTURE_FAIL);
}
