  ClientBuffer(std::unique_ptr<base::SharedMemory> buffer, size_t buffer_size)
      : buffer_(std::move(buffer)), buffer_size_(buffer_size) {}
