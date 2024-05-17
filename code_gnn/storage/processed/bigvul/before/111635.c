void GDataRootDirectory::FindEntryByPath(const FilePath& file_path,
                                         const FindEntryCallback& callback) {
  DCHECK(!callback.is_null());

  std::vector<FilePath::StringType> components;
  file_path.GetComponents(&components);

  GDataDirectory* current_dir = this;
  FilePath directory_path;

  util::GDataSearchPathType path_type =
      util::GetSearchPathStatusForPathComponents(components);

  if (path_type == util::GDATA_SEARCH_PATH_ROOT ||
      path_type == util::GDATA_SEARCH_PATH_QUERY) {
    callback.Run(base::PLATFORM_FILE_OK, file_path.DirName(),
                 fake_search_directory_.get());
    return;
  }

  if (path_type != util::GDATA_SEARCH_PATH_INVALID) {
    if (!ModifyFindEntryParamsForSearchPath(file_path,
             &components, &current_dir, &directory_path)) {
      callback.Run(base::PLATFORM_FILE_ERROR_NOT_FOUND, FilePath(), NULL);
      return;
    }
  }

  for (size_t i = 0; i < components.size() && current_dir; i++) {
    directory_path = directory_path.Append(current_dir->file_name());

    if (i == components.size() - 1) {
      if (current_dir->file_name() == components[i])
        callback.Run(base::PLATFORM_FILE_OK, directory_path, current_dir);
      else
        callback.Run(base::PLATFORM_FILE_ERROR_NOT_FOUND, FilePath(), NULL);
      return;
    }

    GDataEntry* entry = current_dir->FindChild(components[i + 1]);
    if (!entry) {
      callback.Run(base::PLATFORM_FILE_ERROR_NOT_FOUND, FilePath(), NULL);
      return;
    }

    if (entry->file_info().is_directory) {
      current_dir = entry->AsGDataDirectory();
    } else {
      if ((i + 1) == (components.size() - 1))
        callback.Run(base::PLATFORM_FILE_OK, directory_path, entry);
      else
        callback.Run(base::PLATFORM_FILE_ERROR_NOT_FOUND, FilePath(), NULL);

      return;
    }
  }
  callback.Run(base::PLATFORM_FILE_ERROR_NOT_FOUND, FilePath(), NULL);
}
