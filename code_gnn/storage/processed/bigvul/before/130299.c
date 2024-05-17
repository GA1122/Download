void OSExchangeDataProviderWin::SetFilename(const base::FilePath& path) {
  STGMEDIUM* storage = GetStorageForFileName(path);
  DataObjectImpl::StoredDataInfo* info = new DataObjectImpl::StoredDataInfo(
      Clipboard::GetCFHDropFormatType().ToFormatEtc(), storage);
  data_->contents_.push_back(info);

  storage = GetIDListStorageForFileName(path);
  if (!storage)
    return;
  info = new DataObjectImpl::StoredDataInfo(
      Clipboard::GetIDListFormatType().ToFormatEtc(), storage);
  data_->contents_.push_back(info);
}
