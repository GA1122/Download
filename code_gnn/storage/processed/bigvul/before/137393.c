void RenderViewTest::GoToOffset(int offset,
                                const GURL& url,
                                const PageState& state) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);

  int history_list_length =
      impl->HistoryBackListCount() + impl->HistoryForwardListCount() + 1;
  int pending_offset = offset + impl->history_list_offset_;

  CommonNavigationParams common_params(
      url, Referrer(), ui::PAGE_TRANSITION_FORWARD_BACK,
      FrameMsg_Navigate_Type::HISTORY_DIFFERENT_DOCUMENT, true, false,
      base::TimeTicks(), FrameMsg_UILoadMetricsReportType::NO_REPORT, GURL(),
      GURL(), PREVIEWS_UNSPECIFIED, base::TimeTicks::Now(), "GET", nullptr,
      base::Optional<SourceLocation>(),
      CSPDisposition::CHECK  ,
      false  , false  );
  RequestNavigationParams request_params;
  request_params.page_state = state;
  request_params.nav_entry_id = pending_offset + 1;
  request_params.pending_history_list_offset = pending_offset;
  request_params.current_history_list_offset = impl->history_list_offset_;
  request_params.current_history_list_length = history_list_length;

  TestRenderFrame* frame =
      static_cast<TestRenderFrame*>(impl->GetMainRenderFrame());
  frame->Navigate(common_params, request_params);

  FrameLoadWaiter(frame).Wait();
  view_->GetWebView()->UpdateAllLifecyclePhases();
}
