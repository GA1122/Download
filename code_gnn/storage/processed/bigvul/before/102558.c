  virtual void RunWork() {
    base::PlatformFileError error_code(base::PLATFORM_FILE_ERROR_FAILED);
    base::PlatformFile file =
        base::CreatePlatformFile(
            path_,
            base::PLATFORM_FILE_OPEN | base::PLATFORM_FILE_WRITE,
            NULL,
            &error_code);
    if (error_code != base::PLATFORM_FILE_OK) {
      set_error_code(error_code);
      return;
    }
    if (!base::TruncatePlatformFile(file, length_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
    base::ClosePlatformFile(file);
  }
