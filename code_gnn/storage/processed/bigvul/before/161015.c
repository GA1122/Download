void ChromeClientImpl::StartDragging(LocalFrame* frame,
                                     const WebDragData& drag_data,
                                     WebDragOperationsMask mask,
                                     const WebImage& drag_image,
                                     const WebPoint& drag_image_offset) {
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebReferrerPolicy policy = web_frame->GetDocument().GetReferrerPolicy();
  web_frame->LocalRoot()->FrameWidget()->StartDragging(
      policy, drag_data, mask, drag_image, drag_image_offset);
}
