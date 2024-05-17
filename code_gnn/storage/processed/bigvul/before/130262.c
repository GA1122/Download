bool OSExchangeDataProviderWin::GetFilenames(
    std::vector<FileInfo>* filenames) const {
  std::vector<base::string16> filenames_local;
  bool success = ClipboardUtil::GetFilenames(source_object_, &filenames_local);
  if (success) {
    for (size_t i = 0; i < filenames_local.size(); ++i)
      filenames->push_back(
          FileInfo(base::FilePath(filenames_local[i]), base::FilePath()));
  }
  return success;
}
