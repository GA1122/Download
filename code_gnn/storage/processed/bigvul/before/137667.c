WebContents* PrintPreviewDialogController::GetInitiator(
    WebContents* preview_dialog) {
  PrintPreviewDialogMap::iterator it = preview_dialog_map_.find(preview_dialog);
  return (it != preview_dialog_map_.end()) ? it->second : nullptr;
}
