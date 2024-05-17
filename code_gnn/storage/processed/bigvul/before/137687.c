scoped_refptr<base::RefCountedBytes> GetDataFromHandle(
    base::SharedMemoryHandle handle,
    uint32_t data_size) {
  std::unique_ptr<base::SharedMemory> shared_buf(
      new base::SharedMemory(handle, true));
  if (!shared_buf->Map(data_size)) {
    NOTREACHED();
    return nullptr;
  }

  unsigned char* data_begin = static_cast<unsigned char*>(shared_buf->memory());
  std::vector<unsigned char> data(data_begin, data_begin + data_size);
  return base::RefCountedBytes::TakeVector(&data);
}
