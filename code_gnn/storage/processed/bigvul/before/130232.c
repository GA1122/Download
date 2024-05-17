bool HasData(IDataObject* data_object, const Clipboard::FormatType& format) {
  FORMATETC format_etc = format.ToFormatEtc();
  return SUCCEEDED(data_object->QueryGetData(&format_etc));
}
