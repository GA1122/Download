PageState RenderViewTest::GetCurrentPageState() {
  RenderViewImpl* view_impl = static_cast<RenderViewImpl*>(view_);

  TestRenderFrame* frame =
      static_cast<TestRenderFrame*>(view_impl->GetMainRenderFrame());
  return SingleHistoryItemToPageState(frame->current_history_item());
}
