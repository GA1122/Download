  virtual void RunWork() {
    if (!file_util::DirectoryExists(file_path_)) {
      set_error_code(base::PLATFORM_FILE_ERROR_NOT_FOUND);
      return;
    }

    file_util::FileEnumerator file_enum(
        file_path_, false, static_cast<file_util::FileEnumerator::FileType>(
        file_util::FileEnumerator::FILES |
        file_util::FileEnumerator::DIRECTORIES));
    FilePath current;
    while (!(current = file_enum.Next()).empty()) {
      base::FileUtilProxy::Entry entry;
      file_util::FileEnumerator::FindInfo info;
      file_enum.GetFindInfo(&info);
      entry.is_directory = file_enum.IsDirectory(info);
      entry.name = file_util::FileEnumerator::GetFilename(info).value();
      entry.size = file_util::FileEnumerator::GetFilesize(info);
      entry.last_modified_time =
          file_util::FileEnumerator::GetLastModifiedTime(info);
      entries_.push_back(entry);
    }
  }
