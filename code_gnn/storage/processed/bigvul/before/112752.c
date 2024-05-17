void PrintPreviewHandler::HandleHidePreview(const ListValue*  ) {
  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      web_ui()->GetController());
  print_preview_ui->OnHidePreviewTab();
}
