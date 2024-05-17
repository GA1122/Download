void RenderViewImpl::startDragging(WebFrame* frame,
                                   const WebDragData& data,
                                   WebDragOperationsMask mask,
                                   const WebImage& image,
                                   const WebPoint& webImageOffset) {
  WebDropData drop_data(data);
  drop_data.referrer_policy = frame->document().referrerPolicy();
  gfx::Vector2d imageOffset(webImageOffset.x, webImageOffset.y);
  Send(new DragHostMsg_StartDragging(routing_id_,
                                     drop_data,
                                     mask,
                                     image.getSkBitmap(),
                                     imageOffset,
                                     possible_drag_event_info_));
}
