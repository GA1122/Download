static future_t *shut_down(void) {
  btif_config_flush();
 return future_new_immediate(FUTURE_SUCCESS);
}
