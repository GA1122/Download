void PrintWebViewHelper::DidFinishPrinting(PrintingResult result) {
  bool store_print_pages_params = true;
  switch (result) {
    case OK:
      break;

    case FAIL_PRINT_INIT:
      DCHECK(!notify_browser_of_print_failure_);
      break;

    case FAIL_PRINT:
      if (notify_browser_of_print_failure_ && print_pages_params_.get()) {
        int cookie = print_pages_params_->params.document_cookie;
        Send(new PrintHostMsg_PrintingFailed(routing_id(), cookie));
      }
      break;

    case FAIL_PREVIEW:
      DCHECK(is_preview_enabled_);
      store_print_pages_params = false;
      int cookie = print_pages_params_.get() ?
          print_pages_params_->params.document_cookie : 0;
      if (notify_browser_of_print_failure_)
        Send(new PrintHostMsg_PrintPreviewFailed(routing_id(), cookie));
      else
        Send(new PrintHostMsg_PrintPreviewCancelled(routing_id(), cookie));
      print_preview_context_.Failed(notify_browser_of_print_failure_);
      break;
  }

  if (print_web_view_) {
    print_web_view_->close();
    print_web_view_ = NULL;
  }

  if (store_print_pages_params) {
    old_print_pages_params_.reset(print_pages_params_.release());
  } else {
    print_pages_params_.reset();
    old_print_pages_params_.reset();
  }

  notify_browser_of_print_failure_ = true;
}
