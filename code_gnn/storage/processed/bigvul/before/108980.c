void RenderViewImpl::OnSetNavigationStartTime(
    const base::TimeTicks& browser_navigation_start) {
  if (!webview())
    return;

  if (page_id_ != -1)
    return;

  base::TimeTicks navigation_start = std::min(base::TimeTicks::Now(),
                                              browser_navigation_start);
  webview()->mainFrame()->provisionalDataSource()->setNavigationStartTime(
      (navigation_start - base::TimeTicks()).InSecondsF());
}
