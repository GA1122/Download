  virtual void RunWork() {
    if (!file_util::TouchFile(
            file_path_, last_access_time_, last_modified_time_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
