void GlobalHistogramAllocator::ConstructFilePathsForUploadDir(
    const FilePath& active_dir,
    const FilePath& upload_dir,
    const std::string& name,
    FilePath* out_upload_path,
    FilePath* out_active_path,
    FilePath* out_spare_path) {
  if (out_upload_path) {
    *out_upload_path = ConstructFilePathForUploadDir(
        upload_dir, name, Time::Now(), GetCurrentProcId());
  }

  if (out_active_path) {
    *out_active_path =
        ConstructFilePath(active_dir, name + std::string("-active"));
  }

  if (out_spare_path) {
    *out_spare_path =
        ConstructFilePath(active_dir, name + std::string("-spare"));
  }
}
