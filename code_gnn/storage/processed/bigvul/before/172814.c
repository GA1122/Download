static int init(bt_callbacks_t *callbacks) {
  LOG_INFO("%s", __func__);

 if (interface_ready())
 return BT_STATUS_DONE;

#ifdef BLUEDROID_DEBUG
  allocation_tracker_init();
#endif

  bt_hal_cbacks = callbacks;
  stack_manager_get_interface()->init_stack();
  btif_debug_init();

   return BT_STATUS_SUCCESS;
 }
