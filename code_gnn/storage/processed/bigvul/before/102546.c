  virtual void RunWork() {
    if (!file_util::DirectoryExists(file_path_.DirName())) {
      set_error_code(base::PLATFORM_FILE_ERROR_NOT_FOUND);
      return;
    }
    base::PlatformFileError error_code = base::PLATFORM_FILE_OK;
    base::PlatformFile handle = base::CreatePlatformFile(
        file_path_,
        base::PLATFORM_FILE_CREATE | base::PLATFORM_FILE_READ,
        &created_, &error_code);
    if (error_code == base::PLATFORM_FILE_ERROR_EXISTS) {
      created_ = false;
      error_code = base::PLATFORM_FILE_OK;
    }
    if (handle != base::kInvalidPlatformFileValue)
      base::ClosePlatformFile(handle);
    set_error_code(error_code);
  }
