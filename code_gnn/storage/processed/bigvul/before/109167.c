void RenderViewImpl::startDragging(WebFrame* frame,
                                   const WebDragData& data,
                                   WebDragOperationsMask mask,
                                   const WebImage& image,
                                   const WebPoint& imageOffset) {
  WebDropData drop_data(data);
  drop_data.referrer_policy = frame->document().referrerPolicy();
  Send(new DragHostMsg_StartDragging(routing_id_,
                                     drop_data,
                                     mask,
                                     image.getSkBitmap(),
                                     imageOffset));
}
