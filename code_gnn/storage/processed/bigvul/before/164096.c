bool AppCacheDatabase::DeleteExistingAndCreateNewDatabase() {
  DCHECK(!db_file_path_.empty());
  DCHECK(base::PathExists(db_file_path_));
  VLOG(1) << "Deleting existing appcache data and starting over.";

  ResetConnectionAndTables();

  base::FilePath directory = db_file_path_.DirName();
  if (!base::DeleteFile(directory, true))
    return false;

  if (base::PathExists(directory))
    return false;

  if (!base::CreateDirectory(directory))
    return false;

  if (is_recreating_)
    return false;

  base::AutoReset<bool> auto_reset(&is_recreating_, true);
  return LazyOpen(kCreateIfNeeded);
}
