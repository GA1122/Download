bool DragController::StartDrag(LocalFrame* src,
                               const DragState& state,
                               const WebMouseEvent& drag_event,
                               const IntPoint& drag_origin) {
#if DCHECK_IS_ON()
  DCHECK(DragTypeIsValid(state.drag_type_));
#endif
  DCHECK(src);
  if (!src->View() || !src->ContentLayoutObject())
    return false;

  HitTestResult hit_test_result =
      src->GetEventHandler().HitTestResultAtPoint(drag_origin);
  if (!state.drag_src_->IsShadowIncludingInclusiveAncestorOf(
          hit_test_result.InnerNode())) {
    return false;
  }
  const KURL& link_url = hit_test_result.AbsoluteLinkURL();
  const KURL& image_url = hit_test_result.AbsoluteImageURL();

  IntPoint mouse_dragged_point = src->View()->RootFrameToContents(
      FlooredIntPoint(drag_event.PositionInRootFrame()));

  IntPoint drag_location;
  IntPoint drag_offset;

  DataTransfer* data_transfer = state.drag_data_transfer_.Get();
  std::unique_ptr<DragImage> drag_image =
      data_transfer->CreateDragImage(drag_offset, src);
  if (drag_image) {
    drag_location = DragLocationForDHTMLDrag(mouse_dragged_point, drag_origin,
                                             drag_offset, !link_url.IsEmpty());
  }

  Node* node = state.drag_src_.Get();
  if (state.drag_type_ == kDragSourceActionSelection) {
    if (!drag_image) {
      drag_image = DragImageForSelection(*src, kDragImageAlpha);
      drag_location = DragLocationForSelectionDrag(*src);
    }
    DoSystemDrag(drag_image.get(), drag_location, drag_origin, data_transfer,
                 src, false);
  } else if (state.drag_type_ == kDragSourceActionImage) {
    if (image_url.IsEmpty() || !node || !node->IsElementNode())
      return false;
    Element* element = ToElement(node);
    Image* image = GetImage(element);
    if (!image || image->IsNull() || !image->Data() || !image->Data()->size())
      return false;
    DCHECK(!image->FilenameExtension().IsEmpty());
    if (!drag_image) {
      const IntRect& image_rect = hit_test_result.ImageRect();
      IntSize image_size_in_pixels = image_rect.Size();
      image_size_in_pixels.Scale(src->GetPage()->DeviceScaleFactorDeprecated() *
                                 src->GetPage()->GetVisualViewport().Scale());

      float screen_device_scale_factor =
          src->GetPage()->GetChromeClient().GetScreenInfo().device_scale_factor;
      drag_image = DragImageForImage(element, image, screen_device_scale_factor,
                                     drag_origin, image_rect.Location(),
                                     image_size_in_pixels, drag_location);
    }
    DoSystemDrag(drag_image.get(), drag_location, drag_origin, data_transfer,
                 src, false);
  } else if (state.drag_type_ == kDragSourceActionLink) {
    if (link_url.IsEmpty())
      return false;
    if (src->Selection()
            .ComputeVisibleSelectionInDOMTreeDeprecated()
            .IsCaret() &&
        src->Selection()
            .ComputeVisibleSelectionInDOMTreeDeprecated()
            .IsContentEditable()) {
      if (Node* node = EnclosingAnchorElement(
              src->Selection()
                  .ComputeVisibleSelectionInDOMTreeDeprecated()
                  .Base())) {
        src->Selection().SetSelectionAndEndTyping(
            SelectionInDOMTree::Builder().SelectAllChildren(*node).Build());
      }
    }

    if (!drag_image) {
      DCHECK(src->GetPage());
      float screen_device_scale_factor =
          src->GetPage()->GetChromeClient().GetScreenInfo().device_scale_factor;
      drag_image = DragImageForLink(link_url, hit_test_result.TextContent(),
                                    screen_device_scale_factor);
      drag_location = DragLocationForLink(drag_image.get(), mouse_dragged_point,
                                          screen_device_scale_factor,
                                          src->GetPage()->PageScaleFactor());
    }
    DoSystemDrag(drag_image.get(), drag_location, mouse_dragged_point,
                 data_transfer, src, true);
  } else if (state.drag_type_ == kDragSourceActionDHTML) {
    DoSystemDrag(drag_image.get(), drag_location, drag_origin, data_transfer,
                 src, false);
  } else {
    NOTREACHED();
    return false;
  }

  return true;
}
