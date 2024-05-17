void GetFileInfoOnBlockingPool(const FilePath& path,
                               base::PlatformFileInfo* file_info,
                               bool* result) {
  *result = file_util::GetFileInfo(path, file_info);
}
