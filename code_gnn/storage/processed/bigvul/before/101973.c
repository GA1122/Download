PrintWebViewHelper::PrintWebViewHelper(RenderView* render_view)
    : RenderViewObserver(render_view),
      RenderViewObserverTracker<PrintWebViewHelper>(render_view),
      print_web_view_(NULL),
      is_preview_(switches::IsPrintPreviewEnabled()),
      is_print_ready_metafile_sent_(false),
      user_cancelled_scripted_print_count_(0),
      notify_browser_of_print_failure_(true) {
}
