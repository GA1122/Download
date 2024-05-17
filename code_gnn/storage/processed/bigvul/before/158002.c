void RenderViewImpl::UpdateBrowserControlsState(
    BrowserControlsState constraints,
    BrowserControlsState current,
    bool animate) {
  if (GetWebWidget()) {
    GetWebWidget()->UpdateBrowserControlsState(ContentToCc(constraints),
                                               ContentToCc(current), animate);
  }

  top_controls_constraints_ = constraints;
}
