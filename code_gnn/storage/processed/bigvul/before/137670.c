WebContents* PrintPreviewDialogController::GetPrintPreviewForContents(
    WebContents* contents) const {
  PrintPreviewDialogMap::const_iterator it = preview_dialog_map_.find(contents);
  if (it != preview_dialog_map_.end())
    return contents;

  for (it = preview_dialog_map_.begin();
       it != preview_dialog_map_.end();
       ++it) {
    if (contents == it->second) {
      return it->first;
    }
  }
  return nullptr;
}
