static int disable(void) {
 if (!interface_ready())
 return BT_STATUS_NOT_READY;

  stack_manager_get_interface()->shut_down_stack_async();
 return BT_STATUS_SUCCESS;
}
