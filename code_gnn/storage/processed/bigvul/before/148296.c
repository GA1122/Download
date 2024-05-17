PrintViewManager::PrintViewManager(content::WebContents* web_contents)
    : PrintViewManagerBase(web_contents),
      print_preview_state_(NOT_PREVIEWING),
      print_preview_rfh_(nullptr),
      scripted_print_preview_rph_(nullptr) {
  if (PrintPreviewDialogController::IsPrintPreviewDialog(web_contents)) {
    EnableInternalPDFPluginForContents(
        web_contents->GetRenderProcessHost()->GetID(),
        web_contents->GetMainFrame()->GetRoutingID());
  }
}
