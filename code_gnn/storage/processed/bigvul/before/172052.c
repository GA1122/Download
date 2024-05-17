static rfc_slot_t *find_free_slot(void) {
 for (size_t i = 0; i < ARRAY_SIZE(rfc_slots); ++i)
 if (rfc_slots[i].fd == INVALID_FD)
 return &rfc_slots[i];
 return NULL;
}
