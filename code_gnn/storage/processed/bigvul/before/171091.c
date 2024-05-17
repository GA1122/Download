size_t calculate_camera_metadata_size(size_t entry_count,
 size_t data_count) {
 size_t memory_needed = sizeof(camera_metadata_t);
    memory_needed = ALIGN_TO(memory_needed, ENTRY_ALIGNMENT);
    memory_needed += sizeof(camera_metadata_buffer_entry_t[entry_count]);
    memory_needed = ALIGN_TO(memory_needed, DATA_ALIGNMENT);
    memory_needed += sizeof(uint8_t[data_count]);
 return memory_needed;
}
