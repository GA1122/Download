void CleanupOldCacheDir(const base::FilePath& path) {
  if (!base::DirectoryExists(path)) {
    return;
  }

  base::FileEnumerator traversal(
      path, false,
      base::FileEnumerator::FILES | base::FileEnumerator::DIRECTORIES);
  for (base::FilePath current = traversal.Next(); !current.empty();
       current = traversal.Next()) {
    if (traversal.GetInfo().IsDirectory()) {
      return;
    }

    base::FilePath::StringType name = traversal.GetInfo().GetName().value();
    if (name != FILE_PATH_LITERAL("index") &&
        name.compare(0, 5, FILE_PATH_LITERAL("data_")) != 0 &&
        name.compare(0, 2, FILE_PATH_LITERAL("f_")) != 0) {
      return;
    }
  }

  base::DeleteFile(path, true);
}
