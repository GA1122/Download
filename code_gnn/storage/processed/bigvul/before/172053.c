static rfc_slot_t *find_rfc_slot_by_id(uint32_t id) {
  assert(id != 0);

 for (size_t i = 0; i < ARRAY_SIZE(rfc_slots); ++i)
 if (rfc_slots[i].id == id)
 return &rfc_slots[i];

  LOG_ERROR("%s unable to find RFCOMM slot id: %d", __func__, id);
 return NULL;
}
