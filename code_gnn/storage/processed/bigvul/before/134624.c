void OSExchangeDataProviderAura::SetFilenames(
    const std::vector<OSExchangeData::FileInfo>& filenames) {
  filenames_ = filenames;
  formats_ |= OSExchangeData::FILE_NAME;
}
