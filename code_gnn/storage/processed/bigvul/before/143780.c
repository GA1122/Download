void GlobalHistogramAllocator::ConstructFilePaths(const FilePath& dir,
                                                  StringPiece name,
                                                  FilePath* out_base_path,
                                                  FilePath* out_active_path,
                                                  FilePath* out_spare_path) {
  if (out_base_path)
    *out_base_path = ConstructFilePath(dir, name);

  if (out_active_path) {
    *out_active_path =
        ConstructFilePath(dir, name.as_string().append("-active"));
  }

  if (out_spare_path) {
    *out_spare_path = ConstructFilePath(dir, name.as_string().append("-spare"));
  }
}
