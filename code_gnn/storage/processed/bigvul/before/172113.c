static bool stream_corrupted_during_le_scan_workaround(const uint8_t byte_read)
{
 if (!stream_corruption_detected && byte_read == HCI_BLE_EVENT) {
    LOG_ERROR("%s HCI stream corrupted (message type 0x3E)!", __func__);
    stream_corruption_detected = true;
 return true;
 }

 if (stream_corruption_detected) {
 if (stream_corruption_bytes_to_ignore == 0) {
      stream_corruption_bytes_to_ignore = byte_read;
      LOG_ERROR("%s About to skip %d bytes...", __func__, stream_corruption_bytes_to_ignore);
 } else {
 --stream_corruption_bytes_to_ignore;
 }

 if (stream_corruption_bytes_to_ignore == 0) {
      LOG_ERROR("%s Back to our regularly scheduled program...", __func__);
      stream_corruption_detected = false;
 }
 return true;
 }

 return false;
}
