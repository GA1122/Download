bool OSExchangeDataProviderWin::GetFilename(base::FilePath* path) const {
  std::vector<base::string16> filenames;
  bool success = ClipboardUtil::GetFilenames(source_object_, &filenames);
  if (success)
    *path = base::FilePath(filenames[0]);
  return success;
}
