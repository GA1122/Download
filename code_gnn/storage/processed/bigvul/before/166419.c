int GetDangerousFileType(const base::FilePath& file_path) {
  for (size_t i = 0; i < arraysize(kDangerousFileTypes); ++i) {
    if (file_path.MatchesExtension(kDangerousFileTypes[i]))
      return i + 1;
  }
  return 0;   
}
