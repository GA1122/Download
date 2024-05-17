void RecordCompositorSlowScrollMetric(InputHandler::ScrollInputType type,
                                      ScrollThread scroll_thread) {
  bool scroll_on_main_thread = (scroll_thread == MAIN_THREAD);
  if (IsWheelBasedScroll(type)) {
    UMA_HISTOGRAM_BOOLEAN("Renderer4.CompositorWheelScrollUpdateThread",
                          scroll_on_main_thread);
  } else {
    UMA_HISTOGRAM_BOOLEAN("Renderer4.CompositorTouchScrollUpdateThread",
                          scroll_on_main_thread);
  }
}
