JavaScriptAppModalDialog* GetActiveJavaScriptModalDialog(
    ErrorCode* error_code) {
  AppModalDialogQueue* dialog_queue = AppModalDialogQueue::GetInstance();
  if (!dialog_queue->HasActiveDialog() ||
      !dialog_queue->active_dialog()->IsJavaScriptModalDialog()) {
    *error_code = automation::kNoJavaScriptModalDialogOpen;
    return NULL;
  }
  return static_cast<JavaScriptAppModalDialog*>(dialog_queue->active_dialog());
}
