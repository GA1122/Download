bool GetData(IDataObject* data_object,
             const Clipboard::FormatType& format,
             STGMEDIUM* medium) {
  FORMATETC format_etc = format.ToFormatEtc();
  return SUCCEEDED(data_object->GetData(&format_etc, medium));
 }
