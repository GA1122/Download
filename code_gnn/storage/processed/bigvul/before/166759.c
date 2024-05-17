size_t BaseArena::ObjectPayloadSizeForTesting() {
#if DCHECK_IS_ON()
  DCHECK(IsConsistentForGC());
#endif

  size_t object_payload_size = 0;
  for (BasePage* page = first_unswept_page_; page; page = page->Next())
    object_payload_size += page->ObjectPayloadSizeForTesting();
  return object_payload_size;
}
