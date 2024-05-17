bool OSExchangeData::GetFilenames(
    std::vector<FileInfo>* filenames) const {
  return provider_->GetFilenames(filenames);
}
