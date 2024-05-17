  virtual void RunWork() {
    if (!base::TruncatePlatformFile(file_, length_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
