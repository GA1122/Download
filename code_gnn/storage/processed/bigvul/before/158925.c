int PDFiumEngine::Form_Alert(IPDF_JSPLATFORM* param,
                             FPDF_WIDESTRING message,
                             FPDF_WIDESTRING title,
                             int type,
                             int icon) {
  enum AlertType {
    ALERT_TYPE_OK = 0,
    ALERT_TYPE_OK_CANCEL,
    ALERT_TYPE_YES_ON,
    ALERT_TYPE_YES_NO_CANCEL
  };

  enum AlertResult {
    ALERT_RESULT_OK = 1,
    ALERT_RESULT_CANCEL,
    ALERT_RESULT_NO,
    ALERT_RESULT_YES
  };

  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  std::string message_str = WideStringToString(message);
  if (type == ALERT_TYPE_OK) {
    engine->client_->Alert(message_str);
    return ALERT_RESULT_OK;
  }

  bool rv = engine->client_->Confirm(message_str);
  if (type == ALERT_TYPE_OK_CANCEL)
    return rv ? ALERT_RESULT_OK : ALERT_RESULT_CANCEL;
  return rv ? ALERT_RESULT_YES : ALERT_RESULT_NO;
}
