void PrintPreviewDialogController::ForEachPreviewDialog(
    base::Callback<void(content::WebContents*)> callback) {
  for (PrintPreviewDialogMap::const_iterator it = preview_dialog_map_.begin();
       it != preview_dialog_map_.end();
       ++it) {
    callback.Run(it->first);
  }
}
