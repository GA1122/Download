 static int add_camera_metadata_entry_raw(camera_metadata_t *dst,
         uint32_t tag,
         uint8_t  type,
 const void *data,
 size_t data_count) {

 if (dst == NULL) return ERROR;
 if (dst->entry_count == dst->entry_capacity) return ERROR;
 if (data_count && data == NULL) return ERROR;

 size_t data_bytes =
            calculate_camera_metadata_entry_data_size(type, data_count);
 if (data_bytes + dst->data_count > dst->data_capacity) return ERROR;

 size_t data_payload_bytes =
            data_count * camera_metadata_type_size[type];
 camera_metadata_buffer_entry_t *entry = get_entries(dst) + dst->entry_count;
    memset(entry, 0, sizeof(camera_metadata_buffer_entry_t));
    entry->tag = tag;
    entry->type = type;
    entry->count = data_count;

 if (data_bytes == 0) {
        memcpy(entry->data.value, data,
                data_payload_bytes);
 } else {
        entry->data.offset = dst->data_count;
        memcpy(get_data(dst) + entry->data.offset, data,
                data_payload_bytes);
        dst->data_count += data_bytes;
 }
    dst->entry_count++;
    dst->flags &= ~FLAG_SORTED;
    assert(validate_camera_metadata_structure(dst, NULL) == OK);
 return OK;
}
