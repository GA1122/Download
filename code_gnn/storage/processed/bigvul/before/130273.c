bool OSExchangeDataProviderWin::HasCustomFormat(
    const OSExchangeData::CustomFormat& format) const {
  FORMATETC format_etc = format.ToFormatEtc();
  return (source_object_->QueryGetData(&format_etc) == S_OK);
}
