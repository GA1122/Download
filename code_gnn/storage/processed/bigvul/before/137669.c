WebContents* PrintPreviewDialogController::GetOrCreatePreviewDialog(
    WebContents* initiator) {
  DCHECK(initiator);

  WebContents* preview_dialog = GetPrintPreviewForContents(initiator);
  if (!preview_dialog)
    return CreatePrintPreviewDialog(initiator);

  initiator->GetDelegate()->ActivateContents(initiator);
  return preview_dialog;
}
