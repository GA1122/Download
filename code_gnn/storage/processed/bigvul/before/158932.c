FPDF_BOOL PDFiumEngine::Form_PutRequestURL(FPDF_FORMFILLINFO* param,
                                           FPDF_WIDESTRING url,
                                           FPDF_WIDESTRING data,
                                           FPDF_WIDESTRING encode) {
  std::string url_str = WideStringToString(url);
  std::string data_str = WideStringToString(data);
  std::string encode_str = WideStringToString(encode);

  std::string javascript =
      "alert(\"Put:" + url_str + "," + data_str + "," + encode_str + "\")";

  return true;
}
