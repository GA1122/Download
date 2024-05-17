  bool CompareData(const CompoundBuffer& buffer, char* data, int size) {
    scoped_refptr<IOBuffer> buffer_data = buffer.ToIOBufferWithSize();
    return buffer.total_bytes() == size &&
        memcmp(buffer_data->data(), data, size) == 0;
  }
