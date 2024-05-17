void RenderViewImpl::OnResize(const ResizeParams& params) {
  TRACE_EVENT0("renderer", "RenderViewImpl::OnResize");

  if (webview()) {
    webview()->HidePopups();
    if (send_preferred_size_changes_ &&
        webview()->MainFrame()->IsWebLocalFrame()) {
      webview()->MainFrame()->ToWebLocalFrame()->SetCanHaveScrollbars(
          ShouldDisplayScrollbars(params.new_size.width(),
                                  params.new_size.height()));
    }
    if (display_mode_ != params.display_mode) {
      display_mode_ = params.display_mode;
      webview()->SetDisplayMode(display_mode_);
    }
  }

  browser_controls_shrink_blink_size_ =
      params.browser_controls_shrink_blink_size;
  top_controls_height_ = params.top_controls_height;
  bottom_controls_height_ = params.bottom_controls_height;

  if (device_scale_factor_for_testing_) {
    ResizeParams p(params);
    p.screen_info.device_scale_factor = *device_scale_factor_for_testing_;
    p.physical_backing_size =
        gfx::ScaleToCeiledSize(p.new_size, p.screen_info.device_scale_factor);
    RenderWidget::OnResize(p);
  } else {
    RenderWidget::OnResize(params);
  }

  if (params.scroll_focused_node_into_view)
    webview()->ScrollFocusedEditableElementIntoView();
}
