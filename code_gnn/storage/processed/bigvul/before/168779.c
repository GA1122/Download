void RenderWidgetHostImpl::SetInitialRenderSizeParams(
    const ResizeParams& resize_params) {
  resize_ack_pending_ = resize_params.needs_resize_ack;

  old_resize_params_ = std::make_unique<ResizeParams>(resize_params);
}
