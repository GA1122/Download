void AppendStringToBuffer(std::vector<uint8_t>* data,
                          const char* str,
                          size_t len) {
  const base::CheckedNumeric<size_t> old_size = data->size();
  data->resize((old_size + len).ValueOrDie());
  memcpy(data->data() + old_size.ValueOrDie(), str, len);
}
