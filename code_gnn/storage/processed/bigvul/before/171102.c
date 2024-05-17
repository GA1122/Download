size_t get_camera_metadata_compact_size(const camera_metadata_t *metadata) {
 if (metadata == NULL) return ERROR;

 return calculate_camera_metadata_size(metadata->entry_count,
                                          metadata->data_count);
}
