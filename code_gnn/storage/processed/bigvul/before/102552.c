  virtual void RunWork() {
    if (!base::GetPlatformFileInfo(file_, &file_info_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
