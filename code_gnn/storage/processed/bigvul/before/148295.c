bool PrintViewManager::PrintPreviewNow(content::RenderFrameHost* rfh,
                                       bool has_selection) {
  if (print_preview_state_ != NOT_PREVIEWING)
    return false;

  auto message = base::MakeUnique<PrintMsg_InitiatePrintPreview>(
      rfh->GetRoutingID(), has_selection);
  if (!PrintNowInternal(rfh, std::move(message)))
    return false;

  DCHECK(!print_preview_rfh_);
  print_preview_rfh_ = rfh;
  print_preview_state_ = USER_INITIATED_PREVIEW;
  return true;
}
