void Instance::DocumentLoadComplete(int page_count) {
  FormTextFieldFocusChange(false);

  if (progress_bar_.visible())
    progress_bar_.Fade(false, kProgressFadeTimeoutMs);

  DCHECK(document_load_state_ == LOAD_STATE_LOADING);
  document_load_state_ = LOAD_STATE_COMPLETE;
  UserMetricsRecordAction("PDF.LoadSuccess");

  if (did_call_start_loading_) {
    pp::PDF::DidStopLoading(this);
    did_call_start_loading_ = false;
  }

  if (on_load_callback_.is_string())
    ExecuteScript(on_load_callback_);
  if (!IsPrintPreview()) {
    int initial_page = GetInitialPage(url_);
    if (initial_page >= 0)
      ScrollToPage(initial_page);
  }

  if (!full_)
    return;
  if (!pp::PDF::IsAvailable())
    return;

  int content_restrictions =
      CONTENT_RESTRICTION_CUT | CONTENT_RESTRICTION_PASTE;
  if (!engine_->HasPermission(PDFEngine::PERMISSION_COPY))
    content_restrictions |= CONTENT_RESTRICTION_COPY;

  if (!engine_->HasPermission(PDFEngine::PERMISSION_PRINT_LOW_QUALITY) &&
      !engine_->HasPermission(PDFEngine::PERMISSION_PRINT_HIGH_QUALITY)) {
    printing_enabled_ = false;
    if (current_tb_info_ == kPDFToolbarButtons) {
      CreateToolbar(kPDFNoPrintToolbarButtons,
                    arraysize(kPDFNoPrintToolbarButtons));
      UpdateToolbarPosition(false);
      Invalidate(pp::Rect(plugin_size_));
    }
  }

  pp::PDF::SetContentRestriction(this, content_restrictions);

  pp::PDF::HistogramPDFPageCount(this, page_count);
}
