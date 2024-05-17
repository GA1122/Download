void OSExchangeDataProviderWin::MarkOriginatedFromRenderer() {
  STGMEDIUM* storage = GetStorageForString(std::string());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      GetRendererTaintCustomType().ToFormatEtc(), storage));
}
