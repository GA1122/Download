base::FilePath DatabaseUtil::GetFullFilePathForVfsFile(
    DatabaseTracker* db_tracker, const string16& vfs_file_name) {
  string16 origin_identifier;
  string16 database_name;
  string16 sqlite_suffix;
  if (!CrackVfsFileName(vfs_file_name, &origin_identifier,
                        &database_name, &sqlite_suffix)) {
    return base::FilePath();  
  }

  base::FilePath full_path = db_tracker->GetFullDBFilePath(
      origin_identifier, database_name);
  if (!full_path.empty() && !sqlite_suffix.empty()) {
    DCHECK(full_path.Extension().empty());
    full_path = full_path.InsertBeforeExtensionASCII(
        UTF16ToASCII(sqlite_suffix));
  }
  if (full_path.value().find(FILE_PATH_LITERAL("..")) !=
          base::FilePath::StringType::npos)
    return base::FilePath();
  return full_path;
}
