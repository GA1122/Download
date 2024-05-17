void OSExchangeData::SetFileContents(const base::FilePath& filename,
                                     const std::string& file_contents) {
  provider_->SetFileContents(filename, file_contents);
}
