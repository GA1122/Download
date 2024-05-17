SharedMemory::SharedMemory(SharedMemoryHandle handle, bool read_only,
                           ProcessHandle process)
    : mapped_file_(NULL),
      memory_(NULL),
      read_only_(read_only),
      created_size_(0),
      lock_(NULL) {
  ::DuplicateHandle(process, handle,
                    GetCurrentProcess(), &mapped_file_,
                    STANDARD_RIGHTS_REQUIRED |
                    (read_only_ ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS),
                    FALSE, 0);
}
