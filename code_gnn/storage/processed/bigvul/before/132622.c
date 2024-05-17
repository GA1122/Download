BlinkTestRunner::BlinkTestRunner(RenderView* render_view)
    : RenderViewObserver(render_view),
      RenderViewObserverTracker<BlinkTestRunner>(render_view),
      proxy_(NULL),
      focused_view_(NULL),
      is_main_window_(false),
      focus_on_next_commit_(false),
      leak_detector_(new LeakDetector(this)) {
}
