void RenderWidgetHostImpl::SetCursor(const WebCursor& cursor) {
  if (!view_)
    return;
  view_->UpdateCursor(cursor);
}
