bool OSExchangeDataProviderWin::GetString(base::string16* data) const {
   return ClipboardUtil::GetPlainText(source_object_, data);
 }
