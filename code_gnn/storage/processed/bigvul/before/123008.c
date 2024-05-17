void RenderWidgetHostImpl::OnMsgPaintAtSizeAck(int tag, const gfx::Size& size) {
  std::pair<int, gfx::Size> details = std::make_pair(tag, size);
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DID_RECEIVE_PAINT_AT_SIZE_ACK,
      Source<RenderWidgetHost>(this),
      Details<std::pair<int, gfx::Size> >(&details));
}
