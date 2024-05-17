void PrintWebViewHelper::DisplayPrintJobError() {
  WebView* web_view = print_web_view_;
  if (!web_view)
    web_view = render_view()->webview();

  render_view()->runModalAlertDialog(
      web_view->mainFrame(),
      l10n_util::GetStringUTF16(IDS_PRINT_SPOOL_FAILED_ERROR_TEXT));
}
