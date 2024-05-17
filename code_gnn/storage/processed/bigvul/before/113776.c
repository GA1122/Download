void PrintPreviewUI::OnPrintPreviewFailed() {
  handler_->OnPrintPreviewFailed();
  web_ui()->CallJavascriptFunction("printPreviewFailed");
}
