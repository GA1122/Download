  bool GetEntryInfoAndCompare(const FilePath& file_path,
                              GDataEntry* entry) {
    file_system_->GetEntryInfoByPathAsync(
        file_path,
        base::Bind(&CallbackHelper::GetEntryInfoCallback,
                   callback_helper_.get()));
    message_loop_.RunAllPending();

    if (entry == NULL) {
      return (callback_helper_->last_error_ ==
              base::PLATFORM_FILE_ERROR_NOT_FOUND &&
              callback_helper_->entry_proto_ == NULL);
    }

    if (callback_helper_->last_error_ != base::PLATFORM_FILE_OK)
      return false;

    scoped_ptr<GDataEntryProto> entry_proto =
        callback_helper_->entry_proto_.Pass();
    return (entry->resource_id() == entry_proto->resource_id());
  }
