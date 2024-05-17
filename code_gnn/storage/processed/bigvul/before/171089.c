camera_metadata_t *allocate_copy_camera_metadata_checked(
 const camera_metadata_t *src,
 size_t src_size) {

 if (src == NULL) {
 return NULL;
 }

 void *buffer = malloc(src_size);
    memcpy(buffer, src, src_size);

 camera_metadata_t *metadata = (camera_metadata_t*) buffer;
 if (validate_camera_metadata_structure(metadata, &src_size) != OK) {
        free(buffer);
 return NULL;
 }

 return metadata;
}
