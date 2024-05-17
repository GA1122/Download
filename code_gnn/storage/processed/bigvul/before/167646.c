double GetPageScaleFactor(Shell* shell) {
  return RenderWidgetHostImpl::From(
             shell->web_contents()->GetRenderViewHost()->GetWidget())
      ->last_frame_metadata()
      .page_scale_factor;
}
