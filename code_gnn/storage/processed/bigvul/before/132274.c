void UpdateFrameNavigationTiming(WebFrame* frame,
                                 base::TimeTicks browser_navigation_start,
                                 base::TimeTicks renderer_navigation_start) {
  DCHECK(!browser_navigation_start.is_null());
  if (frame->provisionalDataSource()) {
    base::TimeTicks navigation_start = std::min(
        browser_navigation_start, renderer_navigation_start);
    double navigation_start_seconds =
        (navigation_start - base::TimeTicks()).InSecondsF();
    frame->provisionalDataSource()->setNavigationStartTime(
        navigation_start_seconds);
  }
}
