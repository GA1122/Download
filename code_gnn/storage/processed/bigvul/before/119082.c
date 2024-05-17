  bool CacheMemoryRegions() {
    std::string contents;
    if (!ReadProcMaps(&contents)) {
      LOG(ERROR) << "Failed to read /proc/self/maps";
      return false;
    }

    if (!ParseProcMaps(contents, &regions_)) {
      LOG(ERROR) << "Failed to parse the contents of /proc/self/maps";
      return false;
    }

    is_initialized_ = true;
    return true;
  }
