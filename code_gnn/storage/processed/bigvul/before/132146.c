PageState RenderViewTest::GetCurrentPageState() {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  return HistoryEntryToPageState(impl->history_controller()->GetCurrentEntry());
}
