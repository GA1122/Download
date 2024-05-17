bool PrintRenderFrameHelper::PrintPreviewContext::CreatePreviewDocument(
    std::unique_ptr<PrepareFrameAndViewForPrint> prepared_frame,
    const std::vector<int>& pages,
    SkiaDocumentType doc_type) {
  DCHECK_EQ(INITIALIZED, state_);
  state_ = RENDERING;

  prep_frame_view_ = std::move(prepared_frame);
  prep_frame_view_->StartPrinting();

  total_page_count_ = prep_frame_view_->GetExpectedPageCount();
  if (total_page_count_ == 0) {
    LOG(ERROR) << "CreatePreviewDocument got 0 page count";
    set_error(PREVIEW_ERROR_ZERO_PAGES);
    return false;
  }

  metafile_ = base::MakeUnique<PdfMetafileSkia>(doc_type);
  CHECK(metafile_->Init());

  current_page_index_ = 0;
  pages_to_render_ = pages;
  std::sort(pages_to_render_.begin(), pages_to_render_.end());
  pages_to_render_.resize(
      std::unique(pages_to_render_.begin(), pages_to_render_.end()) -
      pages_to_render_.begin());
  pages_to_render_.resize(std::lower_bound(pages_to_render_.begin(),
                                           pages_to_render_.end(),
                                           total_page_count_) -
                          pages_to_render_.begin());
  print_ready_metafile_page_count_ = pages_to_render_.size();
  if (pages_to_render_.empty()) {
    print_ready_metafile_page_count_ = total_page_count_;
    for (int i = 0; i < total_page_count_; ++i)
      pages_to_render_.push_back(i);
  } else if (generate_draft_pages_) {
    int pages_index = 0;
    for (int i = 0; i < total_page_count_; ++i) {
      if (pages_index < print_ready_metafile_page_count_ &&
          i == pages_to_render_[pages_index]) {
        pages_index++;
        continue;
      }
      pages_to_render_.push_back(i);
    }
  }

  document_render_time_ = base::TimeDelta();
  begin_time_ = base::TimeTicks::Now();

  return true;
}
