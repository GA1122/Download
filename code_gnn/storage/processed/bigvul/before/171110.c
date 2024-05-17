size_t get_camera_metadata_size(const camera_metadata_t *metadata) {
 if (metadata == NULL) return ERROR;

 return metadata->size;
}
