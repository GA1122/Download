FPDF_BOOL PDFiumEngine::Form_PostRequestURL(FPDF_FORMFILLINFO* param,
                                            FPDF_WIDESTRING url,
                                            FPDF_WIDESTRING data,
                                            FPDF_WIDESTRING content_type,
                                            FPDF_WIDESTRING encode,
                                            FPDF_WIDESTRING header,
                                            FPDF_BSTR* response) {
  std::string url_str = WideStringToString(url);
  std::string data_str = WideStringToString(data);
  std::string content_type_str = WideStringToString(content_type);
  std::string encode_str = WideStringToString(encode);
  std::string header_str = WideStringToString(header);

  std::string javascript = "alert(\"Post:" + url_str + "," + data_str + "," +
                           content_type_str + "," + encode_str + "," +
                           header_str + "\")";
  return true;
}
