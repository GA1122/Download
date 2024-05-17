void RenderWidgetHostImpl::PaintAtSize(TransportDIB::Handle dib_handle,
                                       int tag,
                                       const gfx::Size& page_size,
                                       const gfx::Size& desired_size) {
  Send(new ViewMsg_PaintAtSize(routing_id_, dib_handle, tag,
                               page_size, desired_size));
}
