  virtual void RunWork() {
    bool path_exists = file_util::PathExists(file_path_);
    if (!recursive_ && !file_util::PathExists(file_path_.DirName())) {
      set_error_code(base::PLATFORM_FILE_ERROR_NOT_FOUND);
      return;
    }
    if (exclusive_ && path_exists) {
      set_error_code(base::PLATFORM_FILE_ERROR_EXISTS);
      return;
    }
    if (path_exists && !file_util::DirectoryExists(file_path_)) {
      set_error_code(base::PLATFORM_FILE_ERROR_EXISTS);
      return;
    }
    if (!file_util::CreateDirectory(file_path_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
