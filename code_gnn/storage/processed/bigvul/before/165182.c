void DragController::DoSystemDrag(DragImage* image,
                                  const IntPoint& drag_location,
                                  const IntPoint& event_pos,
                                  DataTransfer* data_transfer,
                                  LocalFrame* frame,
                                  bool for_link) {
  did_initiate_drag_ = true;
  drag_initiator_ = frame->GetDocument();

  IntPoint adjusted_drag_location =
      frame->View()->ContentsToViewport(drag_location);
  IntPoint adjusted_event_pos = frame->View()->ContentsToViewport(event_pos);
  IntSize offset_size(adjusted_event_pos - adjusted_drag_location);
  WebPoint offset_point(offset_size.Width(), offset_size.Height());
  WebDragData drag_data = data_transfer->GetDataObject()->ToWebDragData();
  WebDragOperationsMask drag_operation_mask =
      static_cast<WebDragOperationsMask>(data_transfer->SourceOperation());
  WebImage drag_image;

  if (image) {
    float resolution_scale = image->ResolutionScale();
    float device_scale_factor =
        page_->GetChromeClient().GetScreenInfo().device_scale_factor;
    if (device_scale_factor != resolution_scale) {
      DCHECK_GT(resolution_scale, 0);
      float scale = device_scale_factor / resolution_scale;
      image->Scale(scale, scale);
    }
    drag_image = image->Bitmap();
  }

  page_->GetChromeClient().StartDragging(frame, drag_data, drag_operation_mask,
                                         drag_image, offset_point);
}
