void OSExchangeDataProviderAura::SetFilename(const base::FilePath& path) {
  filenames_.clear();
  filenames_.push_back(OSExchangeData::FileInfo(path, base::FilePath()));
  formats_ |= OSExchangeData::FILE_NAME;
}
