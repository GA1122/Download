void InsertValueIntoBuffer(std::vector<uint8_t>* data,
                           const T& value,
                           size_t offset) {
  DCHECK_LE(offset + sizeof(T), data->size());
  memcpy(data->data() + offset, &value, sizeof(T));
}
