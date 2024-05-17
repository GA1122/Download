  virtual void RunWork() {
    file_util::CreateTemporaryFile(&file_path_);

    int file_flags =
        base::PLATFORM_FILE_WRITE |
        base::PLATFORM_FILE_TEMPORARY |
        base::PLATFORM_FILE_CREATE_ALWAYS |
        additional_file_flags_;

    base::PlatformFileError error_code = base::PLATFORM_FILE_OK;
    file_handle_ = base::CreatePlatformFile(file_path_, file_flags,
                                            NULL, &error_code);
    set_error_code(error_code);
  }
