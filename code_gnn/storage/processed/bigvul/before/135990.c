  bool PdfIsExpectedToFailLoad(const std::string& pdf_file) {
    const char* kFailingPdfs[] = {
        "pdf_private/cfuzz5.pdf",
        "pdf_private/cfuzz6.pdf",
        "pdf_private/crash-11-14-44.pdf",
        "pdf_private/js.pdf",
        "pdf_private/segv-ecx.pdf",
        "pdf_private/tests.pdf",
    };
    for (size_t i = 0; i < arraysize(kFailingPdfs); ++i) {
      if (kFailingPdfs[i] == pdf_file)
        return true;
    }
    return false;
  }
