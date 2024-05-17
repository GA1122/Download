void OSExchangeDataProviderWin::SetDownloadFileInfo(
    const OSExchangeData::DownloadFileInfo& download) {
  STGMEDIUM* storage = NULL;
  if (!download.filename.empty())
    storage = GetStorageForFileName(download.filename);

  DataObjectImpl::StoredDataInfo* info = new DataObjectImpl::StoredDataInfo(
      Clipboard::GetCFHDropFormatType().ToFormatEtc(), storage);
  info->downloader = download.downloader;
  data_->contents_.push_back(info);
}
