int GetBlockForJpeg(void* param,
                    unsigned long pos,
                    unsigned char* buf,
                    unsigned long size) {
  std::vector<uint8_t>* data_vector = static_cast<std::vector<uint8_t>*>(param);
  if (pos + size < pos || pos + size > data_vector->size())
    return 0;
  memcpy(buf, data_vector->data() + pos, size);
  return 1;
}
