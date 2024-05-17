  virtual void RunWork() {
    if (!base::TouchPlatformFile(file_, last_access_time_, last_modified_time_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
