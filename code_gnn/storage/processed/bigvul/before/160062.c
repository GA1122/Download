int BackendImpl::MaxBuffersSize() {
  static int64_t total_memory = base::SysInfo::AmountOfPhysicalMemory();
  static bool done = false;

  if (!done) {
    const int kMaxBuffersSize = 30 * 1024 * 1024;

    total_memory = total_memory * 2 / 100;
    if (total_memory > kMaxBuffersSize || total_memory <= 0)
      total_memory = kMaxBuffersSize;

    done = true;
  }

  return static_cast<int>(total_memory);
}
