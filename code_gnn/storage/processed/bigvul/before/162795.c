GLuint GetBufferId(const Buffer* buffer) {
  if (buffer)
    return buffer->service_id();
  return 0;
}
