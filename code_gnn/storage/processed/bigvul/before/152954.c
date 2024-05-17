void PDFiumEngine::CalculateVisiblePages() {
  pending_pages_.clear();
  doc_loader_.ClearPendingRequests();

  visible_pages_.clear();
  pp::Rect visible_rect(plugin_size_);
  for (size_t i = 0; i < pages_.size(); ++i) {
    if (visible_rect.Intersects(GetPageScreenRect(i))) {
      visible_pages_.push_back(i);
      CheckPageAvailable(i, &pending_pages_);
    } else {
      if (defer_page_unload_) {
        deferred_page_unloads_.push_back(i);
      } else {
        pages_[i]->Unload();
      }

      if (static_cast<int>(i) == last_page_mouse_down_)
        last_page_mouse_down_ = -1;
    }
  }

  form_highlights_.clear();

  int most_visible_page = visible_pages_.empty() ? -1 : visible_pages_.front();
  if (most_visible_page != -1 && !pages_.empty() &&
      most_visible_page < static_cast<int>(pages_.size()) - 1) {
    pp::Rect rc_first =
        visible_rect.Intersect(GetPageScreenRect(most_visible_page));
    pp::Rect rc_next =
        visible_rect.Intersect(GetPageScreenRect(most_visible_page + 1));
    if (rc_next.height() > rc_first.height())
      most_visible_page++;
  }

  SetCurrentPage(most_visible_page);
}
