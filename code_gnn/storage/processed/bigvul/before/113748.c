base::RefCountedBytes* GetDataFromHandle(base::SharedMemoryHandle handle,
                                         uint32 data_size) {
  scoped_ptr<base::SharedMemory> shared_buf(
      new base::SharedMemory(handle, true));
  if (!shared_buf->Map(data_size)) {
    NOTREACHED();
    return NULL;
  }

  char* preview_data = static_cast<char*>(shared_buf->memory());
  std::vector<unsigned char> data(data_size);
  memcpy(&data[0], preview_data, data_size);
  return base::RefCountedBytes::TakeVector(&data);
}
