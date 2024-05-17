bool ClipboardUtil::HasHtml(IDataObject* data_object) {
  DCHECK(data_object);
  return HasData(data_object, Clipboard::GetHtmlFormatType()) ||
         HasData(data_object, Clipboard::GetTextHtmlFormatType());
}
