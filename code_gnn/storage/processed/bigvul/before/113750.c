PrintPreviewUI* PrintPreviewMessageHandler::GetPrintPreviewUI() {
  TabContentsWrapper* tab = GetPrintPreviewTab();
  if (!tab || !tab->web_contents()->GetWebUI())
    return NULL;
  return static_cast<PrintPreviewUI*>(
      tab->web_contents()->GetWebUI()->GetController());
}
