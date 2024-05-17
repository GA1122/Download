void PrintRenderFrameHelper::DidFinishPrinting(PrintingResult result) {
  int cookie =
      print_pages_params_ ? print_pages_params_->params.document_cookie : 0;
  switch (result) {
    case OK:
      break;

    case FAIL_PRINT_INIT:
      DCHECK(!notify_browser_of_print_failure_);
      break;

    case FAIL_PRINT:
      if (notify_browser_of_print_failure_ && print_pages_params_) {
        Send(new PrintHostMsg_PrintingFailed(routing_id(), cookie));
      }
      break;

#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
    case FAIL_PREVIEW:
      if (!is_print_ready_metafile_sent_) {
        if (notify_browser_of_print_failure_) {
          LOG(ERROR) << "CreatePreviewDocument failed";
          Send(new PrintHostMsg_PrintPreviewFailed(routing_id(), cookie));
        } else {
          Send(new PrintHostMsg_PrintPreviewCancelled(routing_id(), cookie));
        }
      }
      print_preview_context_.Failed(notify_browser_of_print_failure_);
      break;
    case INVALID_SETTINGS:
      Send(new PrintHostMsg_PrintPreviewInvalidPrinterSettings(routing_id(),
                                                               cookie));
      print_preview_context_.Failed(false);
      break;
#endif   
  }
  prep_frame_view_.reset();
  print_pages_params_.reset();
  notify_browser_of_print_failure_ = true;
}
