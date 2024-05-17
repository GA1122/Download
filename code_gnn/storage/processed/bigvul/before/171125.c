static int validate_and_calculate_camera_metadata_entry_data_size(size_t *data_size, uint8_t type,
 size_t data_count) {
 if (type >= NUM_TYPES) return ERROR;

 if (data_count != 0 &&
            camera_metadata_type_size[type] > (SIZE_MAX - DATA_ALIGNMENT + 1) / data_count) {
        android_errorWriteLog(SN_EVENT_LOG_ID, "30741779");
 return ERROR;
 }

 size_t data_bytes = data_count * camera_metadata_type_size[type];

 if (data_size) {
 *data_size = data_bytes <= 4 ? 0 : ALIGN_TO(data_bytes, DATA_ALIGNMENT);
 }

 return OK;
}
