bool OSExchangeDataProviderAuraX11::GetFilename(base::FilePath* path) const {
  std::vector<OSExchangeData::FileInfo> filenames;
  if (GetFilenames(&filenames)) {
    *path = filenames.front().path;
    return true;
  }

  return false;
}
