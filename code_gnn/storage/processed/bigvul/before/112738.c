void PrintPreviewHandler::ActivateInitiatorTabAndClosePreviewTab() {
  TabContents* initiator_tab = GetInitiatorTab();
  if (initiator_tab) {
    WebContents* web_contents = initiator_tab->web_contents();
    web_contents->GetDelegate()->ActivateContents(web_contents);
  }
  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      web_ui()->GetController());
  print_preview_ui->OnClosePrintPreviewTab();
}
