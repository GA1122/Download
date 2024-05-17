void PDFiumEngine::Form_GotoURL(FPDF_FORMFILLINFO* param,
                                FPDF_DOCUMENT document,
                                FPDF_WIDESTRING url) {
  std::string url_str = WideStringToString(url);
}
