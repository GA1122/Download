void RenderWidgetHostImpl::OnMsgGetWindowRect(gfx::Rect* results) {
  if (view_)
    *results = view_->GetViewBounds();
}
