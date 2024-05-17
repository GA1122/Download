std::string GetDocumentMetadata(FPDF_DOCUMENT doc, const std::string& key) {
  size_t size = FPDF_GetMetaText(doc, key.c_str(), nullptr, 0);
  if (size == 0)
    return std::string();

  base::string16 value;
  PDFiumAPIStringBufferSizeInBytesAdapter<base::string16> string_adapter(
      &value, size, false);
  string_adapter.Close(
      FPDF_GetMetaText(doc, key.c_str(), string_adapter.GetData(), size));
  return base::UTF16ToUTF8(value);
}
