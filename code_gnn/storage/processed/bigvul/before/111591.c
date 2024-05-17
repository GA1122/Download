  bool ReadDirectoryAndCompare(const FilePath& file_path,
                               GDataDirectory* directory) {
    file_system_->ReadDirectoryByPathAsync(
        file_path,
        base::Bind(&CallbackHelper::ReadDirectoryCallback,
                   callback_helper_.get()));
    message_loop_.RunAllPending();

    if (directory == NULL) {
      return (callback_helper_->last_error_ ==
              base::PLATFORM_FILE_ERROR_NOT_FOUND &&
              callback_helper_->directory_proto_ == NULL);
    }

    if (callback_helper_->last_error_ != base::PLATFORM_FILE_OK)
      return false;

    scoped_ptr<GDataDirectoryProto> directory_proto =
        callback_helper_->directory_proto_.Pass();
    return (directory->resource_id() ==
            directory_proto->gdata_entry().resource_id());
  }
