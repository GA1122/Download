void PrintPreviewDialogController::EraseInitiatorInfo(
    WebContents* preview_dialog) {
  PrintPreviewDialogMap::iterator it = preview_dialog_map_.find(preview_dialog);
  if (it == preview_dialog_map_.end())
    return;

  RemoveObservers(it->second);
  preview_dialog_map_[preview_dialog] = nullptr;
}
