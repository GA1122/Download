int ReadInternal(ScopedVector<std::string>* pending_data,
                 net::IOBuffer* buffer, int buffer_length) {
  size_t index = 0;
  int offset = 0;
  for (; index < pending_data->size() && offset < buffer_length; ++index) {
    const std::string& chunk = *(*pending_data)[index];
    DCHECK(!chunk.empty());

    size_t bytes_to_read = std::min(
        chunk.size(), static_cast<size_t>(buffer_length - offset));
    std::memmove(buffer->data() + offset, chunk.data(), bytes_to_read);
    offset += bytes_to_read;
    if (bytes_to_read < chunk.size()) {
      (*pending_data)[index]->erase(0, bytes_to_read);
      break;
    }
  }

  pending_data->erase(pending_data->begin(), pending_data->begin() + index);

  return offset;
}
