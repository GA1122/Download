bool RenderWidgetHostImpl::CanPauseForPendingResizeOrRepaints() {
  if (is_hidden())
    return false;

  if (!repaint_ack_pending_ && !resize_ack_pending_)
    return false;

  return true;
}
