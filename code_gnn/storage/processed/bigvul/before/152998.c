FPDF_BOOL PDFiumEngine::Form_PostRequestURL(FPDF_FORMFILLINFO* param,
                                            FPDF_WIDESTRING url,
                                            FPDF_WIDESTRING data,
                                            FPDF_WIDESTRING content_type,
                                            FPDF_WIDESTRING encode,
                                            FPDF_WIDESTRING header,
                                            FPDF_BSTR* response) {
  std::string url_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(url));
  std::string data_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(data));
  std::string content_type_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(content_type));
  std::string encode_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(encode));
  std::string header_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(header));

  std::string javascript = "alert(\"Post:"
      + url_str + "," + data_str + "," + content_type_str + ","
      + encode_str + "," + header_str
      + "\")";
  return true;
}
