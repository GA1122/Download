static camera_metadata_buffer_entry_t *get_entries(
 const camera_metadata_t *metadata) {
 return (camera_metadata_buffer_entry_t*)
 ((uint8_t*)metadata + metadata->entries_start);
}
