bool PrintViewManager::BasicPrint(content::RenderFrameHost* rfh) {
  PrintPreviewDialogController* dialog_controller =
      PrintPreviewDialogController::GetInstance();
  if (!dialog_controller)
    return false;

  content::WebContents* print_preview_dialog =
      dialog_controller->GetPrintPreviewForContents(web_contents());
  if (!print_preview_dialog)
    return PrintNow(rfh);

  if (!print_preview_dialog->GetWebUI())
    return false;

  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      print_preview_dialog->GetWebUI()->GetController());
  print_preview_ui->OnShowSystemDialog();
  return true;
}
