const char* DialogTypeToString(ChromeClient::DialogType dialog_type) {
  switch (dialog_type) {
    case ChromeClient::kAlertDialog:
      return "alert";
    case ChromeClient::kConfirmDialog:
      return "confirm";
    case ChromeClient::kPromptDialog:
      return "prompt";
    case ChromeClient::kPrintDialog:
      return "print";
    case ChromeClient::kHTMLDialog:
      NOTREACHED();
  }
  NOTREACHED();
  return "";
}
