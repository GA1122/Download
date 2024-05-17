void OSExchangeDataProviderWin::SetString(const base::string16& data) {
  STGMEDIUM* storage = GetStorageForString(data);
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      Clipboard::GetPlainTextWFormatType().ToFormatEtc(), storage));

  storage = GetStorageForString(base::UTF16ToUTF8(data));
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      Clipboard::GetPlainTextFormatType().ToFormatEtc(), storage));
}
