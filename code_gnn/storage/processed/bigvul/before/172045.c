void btsock_rfc_cleanup(void) {
  pth = -1;

  pthread_mutex_lock(&slot_lock);
 for (size_t i = 0; i < ARRAY_SIZE(rfc_slots); ++i) {
 if (rfc_slots[i].id)
      cleanup_rfc_slot(&rfc_slots[i]);
    list_free(rfc_slots[i].incoming_queue);
 }
  pthread_mutex_unlock(&slot_lock);
}
