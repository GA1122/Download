void AppendValueToBuffer(std::vector<uint8_t>* data, const T& value) {
  const base::CheckedNumeric<size_t> old_size = data->size();
  data->resize((old_size + sizeof(T)).ValueOrDie());
  memcpy(data->data() + old_size.ValueOrDie(), &value, sizeof(T));
}
