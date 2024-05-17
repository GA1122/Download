InputHandler::ScrollStatus LayerTreeHostImpl::FlingScrollBegin() {
  InputHandler::ScrollStatus scroll_status;
  scroll_status.main_thread_scrolling_reasons =
      MainThreadScrollingReason::kNotScrollingOnMain;
  if (!CurrentlyScrollingNode()) {
    scroll_status.thread = SCROLL_IGNORED;
    scroll_status.main_thread_scrolling_reasons =
        MainThreadScrollingReason::kNoScrollingLayer;
  } else {
    scroll_status.thread = SCROLL_ON_IMPL_THREAD;
  }
  return scroll_status;
}
