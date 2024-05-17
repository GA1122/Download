void OSExchangeDataProviderWin::SetPickledData(
    const OSExchangeData::CustomFormat& format,
    const Pickle& data) {
  STGMEDIUM* storage = GetStorageForBytes(data.data(), data.size());
  data_->contents_.push_back(
      new DataObjectImpl::StoredDataInfo(format.ToFormatEtc(), storage));
}
