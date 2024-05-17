void PDFiumEngine::Form_EmailTo(FPDF_FORMFILLINFO* param,
                                FPDF_FILEHANDLER* file_handler,
                                FPDF_WIDESTRING to,
                                FPDF_WIDESTRING subject,
                                FPDF_WIDESTRING cc,
                                FPDF_WIDESTRING bcc,
                                FPDF_WIDESTRING message) {
  std::string to_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(to));
  std::string subject_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(subject));
  std::string cc_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(cc));
  std::string bcc_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(bcc));
  std::string message_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(message));

  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  engine->client_->Email(to_str, cc_str, bcc_str, subject_str, message_str);
}
