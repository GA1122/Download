void RenderViewImpl::didChangeContentsSize(WebFrame* frame,
                                           const WebSize& size) {
  if (webview()->mainFrame() != frame)
    return;
  WebView* frameView = frame->view();
  if (!frameView)
    return;

  bool has_horizontal_scrollbar = frame->hasHorizontalScrollbar();
  bool has_vertical_scrollbar = frame->hasVerticalScrollbar();

  if (has_horizontal_scrollbar != cached_has_main_frame_horizontal_scrollbar_ ||
      has_vertical_scrollbar != cached_has_main_frame_vertical_scrollbar_) {
    Send(new ViewHostMsg_DidChangeScrollbarsForMainFrame(
          routing_id_, has_horizontal_scrollbar, has_vertical_scrollbar));

    cached_has_main_frame_horizontal_scrollbar_ = has_horizontal_scrollbar;
    cached_has_main_frame_vertical_scrollbar_ = has_vertical_scrollbar;
  }

#if defined(OS_ANDROID)
  ScheduleUpdateFrameInfo();
#endif
}
