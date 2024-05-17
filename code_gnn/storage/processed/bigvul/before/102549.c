  virtual void RunWork() {
    set_error_code(PerformCommonCheckAndPreparationForMoveAndCopy(
        src_file_path_, dest_file_path_));
    if (error_code() != base::PLATFORM_FILE_OK)
      return;
    if (!file_util::Move(src_file_path_, dest_file_path_))
      set_error_code(base::PLATFORM_FILE_ERROR_FAILED);
  }
