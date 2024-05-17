void OSExchangeDataProviderWin::SetFilenames(
    const std::vector<FileInfo>& filenames) {
  for (size_t i = 0; i < filenames.size(); ++i) {
    STGMEDIUM* storage = GetStorageForFileName(filenames[i].path);
    DataObjectImpl::StoredDataInfo* info = new DataObjectImpl::StoredDataInfo(
        Clipboard::GetCFHDropFormatType().ToFormatEtc(), storage);
    data_->contents_.push_back(info);
  }
}
