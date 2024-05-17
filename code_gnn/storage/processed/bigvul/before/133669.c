bool HpackDecoder::DecodeNextHeaderTableSizeUpdate(
    HpackInputStream* input_stream) {
  uint32 size = 0;
  if (!input_stream->DecodeNextUint32(&size)) {
    return false;
  }
  if (size > header_table_.settings_size_bound()) {
    return false;
  }
  header_table_.SetMaxSize(size);
  return true;
}
