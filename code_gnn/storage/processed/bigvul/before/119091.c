  void OpenSymbolFiles() {
#if !defined(NDEBUG)
    std::vector<MappedMemoryRegion>::const_iterator it;
    for (it = regions_.begin(); it != regions_.end(); ++it) {
      const MappedMemoryRegion& region = *it;
      if ((region.permissions & MappedMemoryRegion::READ) ==
              MappedMemoryRegion::READ &&
          (region.permissions & MappedMemoryRegion::WRITE) == 0 &&
          (region.permissions & MappedMemoryRegion::EXECUTE) ==
              MappedMemoryRegion::EXECUTE) {
        if (region.path.empty()) {
          continue;
        }
        if (region.path[0] == '[') {
          continue;
        }
        if (modules_.find(region.path) == modules_.end()) {
          int fd = open(region.path.c_str(), O_RDONLY | O_CLOEXEC);
          if (fd >= 0) {
            modules_.insert(std::make_pair(region.path, fd));
          } else {
            LOG(WARNING) << "Failed to open file: " << region.path
                         << "\n  Error: " << strerror(errno);
          }
        }
      }
    }
#endif   
  }
