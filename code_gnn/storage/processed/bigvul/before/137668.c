PrintPreviewDialogController* PrintPreviewDialogController::GetInstance() {
  if (!g_browser_process)
    return nullptr;
  return g_browser_process->print_preview_dialog_controller();
}
