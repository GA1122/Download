static bool IsMainThreadScrolling(const InputHandler::ScrollStatus& status,
                                  const ScrollNode* scroll_node) {
  if (status.thread == InputHandler::SCROLL_ON_MAIN_THREAD) {
    if (!!scroll_node->main_thread_scrolling_reasons) {
      DCHECK(MainThreadScrollingReason::MainThreadCanSetScrollReasons(
          status.main_thread_scrolling_reasons));
    } else {
      DCHECK(MainThreadScrollingReason::CompositorCanSetScrollReasons(
          status.main_thread_scrolling_reasons));
    }
    return true;
  }
  return false;
}
