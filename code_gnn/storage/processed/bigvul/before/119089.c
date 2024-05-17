  void Init() {
    if (CacheMemoryRegions()) {
      OpenSymbolFiles();
      google::InstallSymbolizeOpenObjectFileCallback(
          &OpenObjectFileContainingPc);
    }
  }
