void PrintViewManager::RenderFrameCreated(
    content::RenderFrameHost* render_frame_host) {
  if (PrintPreviewDialogController::IsPrintPreviewDialog(web_contents())) {
    EnableInternalPDFPluginForContents(render_frame_host->GetProcess()->GetID(),
                                       render_frame_host->GetRoutingID());
  }
}
