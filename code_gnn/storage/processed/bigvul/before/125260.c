bool DatabaseUtil::CrackVfsFileName(const string16& vfs_file_name,
                                    string16* origin_identifier,
                                    string16* database_name,
                                    string16* sqlite_suffix) {
  DCHECK(!vfs_file_name.empty());
  size_t first_slash_index = vfs_file_name.find('/');
  size_t last_pound_index = vfs_file_name.rfind('#');
  if ((first_slash_index == string16::npos) ||
      (last_pound_index == string16::npos) ||
      (first_slash_index == 0) ||
      (first_slash_index > last_pound_index)) {
    return false;
  }

  if (origin_identifier)
    *origin_identifier = vfs_file_name.substr(0, first_slash_index);
  if (database_name) {
    *database_name = vfs_file_name.substr(
        first_slash_index + 1, last_pound_index - first_slash_index - 1);
  }
  if (sqlite_suffix) {
    *sqlite_suffix = vfs_file_name.substr(
        last_pound_index + 1, vfs_file_name.length() - last_pound_index - 1);
  }
  return true;
}
