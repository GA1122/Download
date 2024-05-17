void PrintPreviewDialogController::RemovePreviewDialog(
    WebContents* preview_dialog) {
  WebContents* initiator = GetInitiator(preview_dialog);
  if (initiator) {
    RemoveObservers(initiator);
    PrintViewManager::FromWebContents(initiator)->PrintPreviewDone();
  }

  preview_dialog_map_.erase(preview_dialog);
  RemoveObservers(preview_dialog);
}
