SharedMemory::SharedMemory(const std::wstring& name)
    : mapped_file_(NULL),
      memory_(NULL),
      read_only_(false),
      created_size_(0),
      lock_(NULL),
      name_(name) {
}
