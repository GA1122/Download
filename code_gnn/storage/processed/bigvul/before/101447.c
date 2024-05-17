DirectoryBackingStore::DirectoryBackingStore(const string& dir_name,
                                             const FilePath& backing_filepath)
    : load_dbhandle_(NULL),
      save_dbhandle_(NULL),
      dir_name_(dir_name),
      backing_filepath_(backing_filepath),
      needs_column_refresh_(false) {
}
