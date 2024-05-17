Node* DragController::DraggableNode(const LocalFrame* src,
                                    Node* start_node,
                                    const IntPoint& drag_origin,
                                    SelectionDragPolicy selection_drag_policy,
                                    DragSourceAction& drag_type) const {
  if (src->Selection().Contains(drag_origin)) {
    drag_type = kDragSourceActionSelection;
    if (selection_drag_policy == kImmediateSelectionDragResolution)
      return start_node;
  } else {
    drag_type = kDragSourceActionNone;
  }

  Node* node = nullptr;
  DragSourceAction candidate_drag_type = kDragSourceActionNone;
  for (const LayoutObject* layout_object = start_node->GetLayoutObject();
       layout_object; layout_object = layout_object->Parent()) {
    node = layout_object->NonPseudoNode();
    if (!node) {
      continue;
    }
    if (drag_type != kDragSourceActionSelection &&
        SelectTextInsteadOfDrag(*node)) {
      return nullptr;
    }
    if (node->IsElementNode()) {
      EUserDrag drag_mode = layout_object->Style()->UserDrag();
      if (drag_mode == EUserDrag::kNone)
        continue;
      if (layout_object->IsImage() && src->GetSettings() &&
          src->GetSettings()->GetLoadsImagesAutomatically()) {
        drag_type = kDragSourceActionImage;
        return node;
      }
      if (drag_mode == EUserDrag::kElement) {
        candidate_drag_type = kDragSourceActionDHTML;
        break;
      }
      if (IsHTMLAnchorElement(*node) &&
          ToHTMLAnchorElement(node)->IsLiveLink()) {
        candidate_drag_type = kDragSourceActionLink;
        break;
      }
    }
  }

  if (candidate_drag_type == kDragSourceActionNone) {
    return nullptr;
  }

  DCHECK(node);
  if (drag_type == kDragSourceActionSelection) {
    DCHECK_EQ(selection_drag_policy, kDelayedSelectionDragResolution);
    node = start_node;
  } else {
    DCHECK_EQ(drag_type, kDragSourceActionNone);
    drag_type = candidate_drag_type;
  }
  return node;
}
