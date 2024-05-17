base::FilePath GetWebKitRootDirFilePath() {
  base::FilePath base_path;
  PathService::Get(base::DIR_SOURCE_ROOT, &base_path);
  return base_path.Append(FILE_PATH_LITERAL("third_party/WebKit"));
}
