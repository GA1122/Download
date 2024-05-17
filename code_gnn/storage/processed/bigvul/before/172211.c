static void unregister_cb(void *context) {
 unregister_arg_t *arg = (unregister_arg_t *)context;
  reactor_unregister(arg->object);
  reactor_stop(arg->reactor);
}
