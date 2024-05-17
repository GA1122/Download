bool RenderWidgetHostImpl::GetResizeParams(ResizeParams* resize_params) {
  *resize_params = ResizeParams();

  GetWebScreenInfo(&resize_params->screen_info);
  if (delegate_) {
    resize_params->resizer_rect = delegate_->GetRootWindowResizerRect(this);
    resize_params->is_fullscreen_granted =
        delegate_->IsFullscreenForCurrentTab(this);
    resize_params->display_mode = delegate_->GetDisplayMode(this);
  } else {
    resize_params->is_fullscreen_granted = false;
    resize_params->display_mode = blink::WebDisplayModeBrowser;
  }

  if (view_) {
    resize_params->new_size = view_->GetRequestedRendererSize();
    resize_params->physical_backing_size = view_->GetPhysicalBackingSize();
    resize_params->top_controls_height = view_->GetTopControlsHeight();
    resize_params->top_controls_shrink_blink_size =
        view_->DoTopControlsShrinkBlinkSize();
    resize_params->visible_viewport_size = view_->GetVisibleViewportSize();
  }

  const bool size_changed =
      !old_resize_params_ ||
      old_resize_params_->new_size != resize_params->new_size ||
      (old_resize_params_->physical_backing_size.IsEmpty() &&
       !resize_params->physical_backing_size.IsEmpty());
  bool dirty = size_changed ||
      old_resize_params_->screen_info != resize_params->screen_info ||
      old_resize_params_->physical_backing_size !=
          resize_params->physical_backing_size ||
      old_resize_params_->is_fullscreen_granted !=
          resize_params->is_fullscreen_granted ||
      old_resize_params_->display_mode != resize_params->display_mode ||
      old_resize_params_->top_controls_height !=
          resize_params->top_controls_height ||
      old_resize_params_->top_controls_shrink_blink_size !=
          resize_params->top_controls_shrink_blink_size ||
      old_resize_params_->visible_viewport_size !=
          resize_params->visible_viewport_size;

  resize_params->needs_resize_ack =
      g_check_for_pending_resize_ack && !resize_params->new_size.IsEmpty() &&
      !resize_params->physical_backing_size.IsEmpty() && size_changed;

  return dirty;
}
