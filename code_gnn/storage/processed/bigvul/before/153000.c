FPDF_BOOL PDFiumEngine::Form_PutRequestURL(FPDF_FORMFILLINFO* param,
                                           FPDF_WIDESTRING url,
                                           FPDF_WIDESTRING data,
                                           FPDF_WIDESTRING encode) {
  std::string url_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(url));
  std::string data_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(data));
  std::string encode_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(encode));

  std::string javascript = "alert(\"Put:"
      + url_str + "," + data_str + "," + encode_str
      + "\")";

  return true;
}
