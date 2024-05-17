int delete_camera_metadata_entry(camera_metadata_t *dst,
 size_t index) {
 if (dst == NULL) return ERROR;
 if (index >= dst->entry_count) return ERROR;

 camera_metadata_buffer_entry_t *entry = get_entries(dst) + index;
 size_t data_bytes = calculate_camera_metadata_entry_data_size(entry->type,
            entry->count);

 if (data_bytes > 0) {
 uint8_t *start = get_data(dst) + entry->data.offset;
 uint8_t *end = start + data_bytes;
 size_t length = dst->data_count - entry->data.offset - data_bytes;
        memmove(start, end, length);

 camera_metadata_buffer_entry_t *e = get_entries(dst);
 size_t i;
 for (i = 0; i < dst->entry_count; i++) {
 if (calculate_camera_metadata_entry_data_size(
                    e->type, e->count) > 0 &&
                    e->data.offset > entry->data.offset) {
                e->data.offset -= data_bytes;
 }
 ++e;
 }
        dst->data_count -= data_bytes;
 }
    memmove(entry, entry + 1,
 sizeof(camera_metadata_buffer_entry_t) *
 (dst->entry_count - index - 1) );
    dst->entry_count -= 1;

    assert(validate_camera_metadata_structure(dst, NULL) == OK);
 return OK;
}
