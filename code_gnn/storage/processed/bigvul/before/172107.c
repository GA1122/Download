static bool hal_init(const hci_hal_callbacks_t *upper_callbacks, thread_t *upper_thread) {
  assert(upper_callbacks != NULL);
  assert(upper_thread != NULL);

  callbacks = upper_callbacks;
  thread = upper_thread;
 return true;
}
