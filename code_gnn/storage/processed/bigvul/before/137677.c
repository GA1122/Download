void PrintPreviewDialogController::PrintPreview(WebContents* initiator) {
  if (initiator->ShowingInterstitialPage() || initiator->IsCrashed())
    return;

  PrintPreviewDialogController* dialog_controller = GetInstance();
  if (!dialog_controller)
    return;
  if (!dialog_controller->GetOrCreatePreviewDialog(initiator)) {
    PrintViewManager* print_view_manager =
        PrintViewManager::FromWebContents(initiator);
    if (print_view_manager)
      print_view_manager->PrintPreviewDone();
  }
}
