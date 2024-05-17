camera_metadata_t* copy_camera_metadata(void *dst, size_t dst_size,
 const camera_metadata_t *src) {
 size_t memory_needed = get_camera_metadata_compact_size(src);

 if (dst == NULL) return NULL;
 if (dst_size < memory_needed) return NULL;

 camera_metadata_t *metadata =
        place_camera_metadata(dst, dst_size, src->entry_count, src->data_count);

    metadata->flags = src->flags;
    metadata->entry_count = src->entry_count;
    metadata->data_count = src->data_count;

    memcpy(get_entries(metadata), get_entries(src),
 sizeof(camera_metadata_buffer_entry_t[metadata->entry_count]));
    memcpy(get_data(metadata), get_data(src),
 sizeof(uint8_t[metadata->data_count]));

    assert(validate_camera_metadata_structure(metadata, NULL) == OK);
 return metadata;
}
