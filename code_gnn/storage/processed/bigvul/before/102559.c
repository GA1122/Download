  virtual void RunWork() {
    if (!base::FlushPlatformFile(file_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
