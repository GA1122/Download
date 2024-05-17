bool DragController::PopulateDragDataTransfer(LocalFrame* src,
                                              const DragState& state,
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

  DataTransfer* data_transfer = state.drag_data_transfer_.Get();
  Node* node = state.drag_src_.Get();

  if (IsHTMLAnchorElement(*node) && ToHTMLAnchorElement(node)->IsLiveLink() &&
      !link_url.IsEmpty()) {
    data_transfer->WriteURL(node, link_url,
                            hit_test_result.TextContent().SimplifyWhiteSpace());
  }

  if (state.drag_type_ == kDragSourceActionSelection) {
    data_transfer->WriteSelection(src->Selection());
  } else if (state.drag_type_ == kDragSourceActionImage) {
    if (image_url.IsEmpty() || !node || !node->IsElementNode())
      return false;
    Element* element = ToElement(node);
    PrepareDataTransferForImageDrag(src, data_transfer, element, link_url,
                                    image_url,
                                    hit_test_result.AltDisplayString());
  } else if (state.drag_type_ == kDragSourceActionLink) {
    if (link_url.IsEmpty())
      return false;
  } else if (state.drag_type_ == kDragSourceActionDHTML) {
    LayoutObject* layout_object = node->GetLayoutObject();
    if (!layout_object) {
      return false;
    }

    IntRect bounding_including_descendants =
        layout_object->AbsoluteBoundingBoxRectIncludingDescendants();
    IntSize delta = drag_origin - bounding_including_descendants.Location();
    data_transfer->SetDragImageElement(node, IntPoint(delta));

  }
  return true;
}
