bool OSExchangeData::GetFileContents(base::FilePath* filename,
                                     std::string* file_contents) const {
  return provider_->GetFileContents(filename, file_contents);
}
