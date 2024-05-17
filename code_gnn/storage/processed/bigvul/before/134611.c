bool OSExchangeDataProviderAura::GetFilenames(
    std::vector<OSExchangeData::FileInfo>* filenames) const {
  if ((formats_ & OSExchangeData::FILE_NAME) == 0)
    return false;
  *filenames = filenames_;
  return true;
}
