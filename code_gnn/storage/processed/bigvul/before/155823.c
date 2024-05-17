base::FilePath GetBlacklistPath() {
  base::FilePath blacklist_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &blacklist_dir);
  return blacklist_dir.AppendASCII(kBlacklistFilename);
}
