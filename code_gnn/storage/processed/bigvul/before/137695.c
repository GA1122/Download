void PrintPreviewMessageHandler::OnSetOptionsFromDocument(
    const PrintHostMsg_SetOptionsFromDocument_Params& params) {
  PrintPreviewUI* print_preview_ui = GetPrintPreviewUI();
  if (!print_preview_ui)
    return;
   print_preview_ui->OnSetOptionsFromDocument(params);
 }
