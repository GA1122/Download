camera_metadata_t *allocate_camera_metadata(size_t entry_capacity,
 size_t data_capacity) {

 size_t memory_needed = calculate_camera_metadata_size(entry_capacity,
                                                          data_capacity);
 void *buffer = malloc(memory_needed);
 return place_camera_metadata(buffer, memory_needed,
                                 entry_capacity,
                                 data_capacity);
}
