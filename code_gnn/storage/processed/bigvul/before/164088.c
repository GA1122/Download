AppCacheDatabase::AppCacheDatabase(const base::FilePath& path)
    : db_file_path_(path),
      is_disabled_(false),
      is_recreating_(false),
      was_corruption_detected_(false) {
}
