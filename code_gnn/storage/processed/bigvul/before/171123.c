camera_metadata_t *clone_camera_metadata(const camera_metadata_t *src) {
 int res;
 if (src == NULL) return NULL;
 camera_metadata_t *clone = allocate_camera_metadata(
        get_camera_metadata_entry_count(src),
        get_camera_metadata_data_count(src));
 if (clone != NULL) {
        res = append_camera_metadata(clone, src);
 if (res != OK) {
            free_camera_metadata(clone);
            clone = NULL;
 }
 }
    assert(validate_camera_metadata_structure(clone, NULL) == OK);
 return clone;
}
