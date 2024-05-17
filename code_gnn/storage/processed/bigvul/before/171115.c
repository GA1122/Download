camera_metadata_t *place_camera_metadata(void *dst,
 size_t dst_size,
 size_t entry_capacity,
 size_t data_capacity) {
 if (dst == NULL) return NULL;

 size_t memory_needed = calculate_camera_metadata_size(entry_capacity,
                                                          data_capacity);
 if (memory_needed > dst_size) return NULL;

 camera_metadata_t *metadata = (camera_metadata_t*)dst;
    metadata->version = CURRENT_METADATA_VERSION;
    metadata->flags = 0;
    metadata->entry_count = 0;
    metadata->entry_capacity = entry_capacity;
    metadata->entries_start =
            ALIGN_TO(sizeof(camera_metadata_t), ENTRY_ALIGNMENT);
    metadata->data_count = 0;
    metadata->data_capacity = data_capacity;
    metadata->size = memory_needed;
 size_t data_unaligned = (uint8_t*)(get_entries(metadata) +
            metadata->entry_capacity) - (uint8_t*)metadata;
    metadata->data_start = ALIGN_TO(data_unaligned, DATA_ALIGNMENT);

    assert(validate_camera_metadata_structure(metadata, NULL) == OK);
 return metadata;
}
