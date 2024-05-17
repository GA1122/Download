void RenderViewTest::ClearHistory() {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->page_id_ = -1;
  impl->history_list_offset_ = -1;
  impl->history_list_length_ = 0;
  impl->history_page_ids_.clear();
}
