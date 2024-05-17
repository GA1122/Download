  static int OpenObjectFileContainingPc(uint64_t pc, uint64_t& start_address,
                                        uint64_t& base_address, char* file_path,
                                        int file_path_size) {
    SandboxSymbolizeHelper* instance = GetInstance();

    std::vector<MappedMemoryRegion>::const_iterator it;
    bool is_first = true;
    for (it = instance->regions_.begin(); it != instance->regions_.end();
         ++it, is_first = false) {
      const MappedMemoryRegion& region = *it;
      if (region.start <= pc && pc < region.end) {
        start_address = region.start;
        base_address = (is_first ? 0U : start_address) - region.offset;
        if (file_path && file_path_size > 0) {
          strncpy(file_path, region.path.c_str(), file_path_size);
          file_path[file_path_size - 1] = '\0';
        }
        return instance->GetFileDescriptor(region.path.c_str());
      }
    }
    return -1;
  }
