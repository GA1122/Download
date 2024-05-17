int find_camera_metadata_ro_entry(const camera_metadata_t *src,
 uint32_t tag,
 camera_metadata_ro_entry_t *entry) {
 return find_camera_metadata_entry((camera_metadata_t*)src, tag,
 (camera_metadata_entry_t*)entry);
}
