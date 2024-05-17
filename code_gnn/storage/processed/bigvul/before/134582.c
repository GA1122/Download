 bool OSExchangeData::GetFilename(base::FilePath* path) const {
  return provider_->GetFilename(path);
}
