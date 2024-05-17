void OSExchangeDataProviderWin::SetURL(const GURL& url,
                                       const base::string16& title) {

  base::string16 x_moz_url_str = base::UTF8ToUTF16(url.spec());
  x_moz_url_str += '\n';
  x_moz_url_str += title;
  STGMEDIUM* storage = GetStorageForString(x_moz_url_str);
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      Clipboard::GetMozUrlFormatType().ToFormatEtc(), storage));

  base::string16 valid_file_name;
  CreateValidFileNameFromTitle(url, title, &valid_file_name);
  std::string shortcut_url_file_contents;
  GetInternetShortcutFileContents(url, &shortcut_url_file_contents);
  SetFileContents(base::FilePath(valid_file_name), shortcut_url_file_contents);

  storage = GetStorageForString(base::UTF8ToUTF16(url.spec()));
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      Clipboard::GetUrlWFormatType().ToFormatEtc(), storage));
  storage = GetStorageForString(url.spec());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      Clipboard::GetUrlFormatType().ToFormatEtc(), storage));


  SetString(base::UTF8ToUTF16(url.spec()));
}
