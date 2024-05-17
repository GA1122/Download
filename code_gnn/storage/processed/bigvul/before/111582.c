  bool GetFileInfoAndCompare(const FilePath& file_path,
                             GDataFile* file) {
    file_system_->GetFileInfoByPathAsync(
        file_path,
        base::Bind(&CallbackHelper::GetFileInfoCallback,
                   callback_helper_.get()));
    message_loop_.RunAllPending();

    if (file == NULL) {
      return (callback_helper_->last_error_ ==
              base::PLATFORM_FILE_ERROR_NOT_FOUND &&
              callback_helper_->file_proto_ == NULL);
    }

    if (callback_helper_->last_error_ != base::PLATFORM_FILE_OK)
      return false;

    scoped_ptr<GDataFileProto> file_proto =
        callback_helper_->file_proto_.Pass();
    return (file->resource_id() == file_proto->gdata_entry().resource_id());
  }
