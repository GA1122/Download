void OSExchangeDataProviderAuraX11::SetFilename(const base::FilePath& path) {
  std::vector<OSExchangeData::FileInfo> data;
  data.push_back(OSExchangeData::FileInfo(path, base::FilePath()));
  SetFilenames(data);
}
