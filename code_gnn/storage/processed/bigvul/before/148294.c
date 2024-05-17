void PrintViewManager::PrintPreviewForWebNode(content::RenderFrameHost* rfh) {
  if (print_preview_state_ != NOT_PREVIEWING)
    return;

  DCHECK(rfh);
  DCHECK(!print_preview_rfh_);
  print_preview_rfh_ = rfh;
  print_preview_state_ = USER_INITIATED_PREVIEW;
}
