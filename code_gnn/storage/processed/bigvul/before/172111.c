static void packet_finished(serial_data_type_t type) {
 if (!stream_has_interpretation)
    LOG_ERROR("%s with no existing stream interpretation.", __func__);
 else if (current_data_type != type)
    LOG_ERROR("%s with different type than existing interpretation.", __func__);

  stream_has_interpretation = false;
}
