void RenderViewImpl::didScrollWithKeyboard(const blink::WebSize& delta) {
  if (delta.height == 0)
    return;

  BrowserControlsState current = delta.height < 0
                                     ? BROWSER_CONTROLS_STATE_SHOWN
                                     : BROWSER_CONTROLS_STATE_HIDDEN;

  UpdateBrowserControlsState(top_controls_constraints_, current, true);
}
