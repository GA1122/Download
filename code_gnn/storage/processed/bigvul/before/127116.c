void SharedMemory::Close() {
  if (memory_ != NULL) {
    UnmapViewOfFile(memory_);
    memory_ = NULL;
  }

  if (mapped_file_ != NULL) {
    CloseHandle(mapped_file_);
    mapped_file_ = NULL;
  }
}
