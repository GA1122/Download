bool MoveCustomWallpaperDirectory(const char* sub_dir,
                                  const std::string& from_name,
                                  const std::string& to_name) {
  base::FilePath base_path =
      WallpaperManagerBase::GetCustomWallpaperDir(sub_dir);
  base::FilePath to_path = base_path.Append(to_name);
  base::FilePath from_path = base_path.Append(from_name);
  if (base::PathExists(from_path))
    return base::Move(from_path, to_path);
  return false;
}
