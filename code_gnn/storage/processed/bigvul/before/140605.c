void OutOfProcessInstance::DocumentLoadComplete(int page_count) {
  FormTextFieldFocusChange(false);

  DCHECK(document_load_state_ == LOAD_STATE_LOADING);
  document_load_state_ = LOAD_STATE_COMPLETE;
  UserMetricsRecordAction("PDF.LoadSuccess");

  if (IsPrintPreview()) {
    AppendBlankPrintPreviewPages();
    OnGeometryChanged(0, 0);
  }

  pp::VarDictionary bookmarks_message;
  bookmarks_message.Set(pp::Var(kType), pp::Var(kJSBookmarksType));
  bookmarks_message.Set(pp::Var(kJSBookmarks), engine_->GetBookmarks());
  PostMessage(bookmarks_message);

  pp::VarDictionary progress_message;
  progress_message.Set(pp::Var(kType), pp::Var(kJSLoadProgressType));
  progress_message.Set(pp::Var(kJSProgressPercentage), pp::Var(100));
  PostMessage(progress_message);

  if (!full_)
    return;

  if (did_call_start_loading_) {
    pp::PDF::DidStopLoading(this);
    did_call_start_loading_ = false;
  }

  int content_restrictions =
      CONTENT_RESTRICTION_CUT | CONTENT_RESTRICTION_PASTE;
  if (!engine_->HasPermission(PDFEngine::PERMISSION_COPY))
    content_restrictions |= CONTENT_RESTRICTION_COPY;

  if (!engine_->HasPermission(PDFEngine::PERMISSION_PRINT_LOW_QUALITY) &&
      !engine_->HasPermission(PDFEngine::PERMISSION_PRINT_HIGH_QUALITY)) {
    content_restrictions |= CONTENT_RESTRICTION_PRINT;
  }

  pp::PDF::SetContentRestriction(this, content_restrictions);

  uma_.HistogramCustomCounts("PDF.PageCount", page_count, 1, 1000000, 50);
}
