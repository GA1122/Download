static void entry_free(void *ptr) {
 if (!ptr)
 return;

 entry_t *entry = ptr;
  osi_free(entry->key);
  osi_free(entry->value);
  osi_free(entry);
}
