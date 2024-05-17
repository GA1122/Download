void WebContentsViewAura::OnOverscrollComplete(OverscrollMode mode) {
  UMA_HISTOGRAM_ENUMERATION("Overscroll.Completed", mode, OVERSCROLL_COUNT);
  OverscrollUpdateForWebContentsDelegate(0);
  NavigationControllerImpl& controller = web_contents_->GetController();
  if (ShouldNavigateForward(controller, mode) ||
      ShouldNavigateBack(controller, mode)) {
    CompleteOverscrollNavigation(mode);
    return;
  }

  ResetOverscrollTransform();
}
