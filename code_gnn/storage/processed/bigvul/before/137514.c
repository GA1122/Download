void PrintWebViewHelper::DidFinishPrinting(PrintingResult result) {
  switch (result) {
    case OK:
      break;

    case FAIL_PRINT_INIT:
      DCHECK(!notify_browser_of_print_failure_);
      break;

    case FAIL_PRINT:
      if (notify_browser_of_print_failure_ && print_pages_params_) {
        int cookie = print_pages_params_->params.document_cookie;
        Send(new PrintHostMsg_PrintingFailed(routing_id(), cookie));
      }
      break;

    case FAIL_PREVIEW:
      int cookie =
          print_pages_params_ ? print_pages_params_->params.document_cookie : 0;
      if (notify_browser_of_print_failure_) {
        LOG(ERROR) << "CreatePreviewDocument failed";
        Send(new PrintHostMsg_PrintPreviewFailed(routing_id(), cookie));
      } else {
        Send(new PrintHostMsg_PrintPreviewCancelled(routing_id(), cookie));
      }
      print_preview_context_.Failed(notify_browser_of_print_failure_);
      break;
  }
  prep_frame_view_.reset();
  print_pages_params_.reset();
  notify_browser_of_print_failure_ = true;
}
