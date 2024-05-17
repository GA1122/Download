void PrintPreviewUI::OnShowSystemDialog() {
  web_ui()->CallJavascriptFunction("onSystemDialogLinkClicked");
}
