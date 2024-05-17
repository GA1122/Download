int get_camera_metadata_ro_entry(const camera_metadata_t *src,
 size_t index,
 camera_metadata_ro_entry_t *entry) {
 return get_camera_metadata_entry((camera_metadata_t*)src, index,
 (camera_metadata_entry_t*)entry);
}
