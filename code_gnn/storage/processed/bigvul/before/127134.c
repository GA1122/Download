bool ReturnsInvalidPath(int dir_type) {
  base::FilePath path;
  bool result = PathService::Get(dir_type, &path);
  return !result && path.empty();
}
