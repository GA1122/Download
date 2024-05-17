PaintLayerScrollableArea::PaintLayerScrollableArea(PaintLayer& layer)
    : layer_(&layer),
      in_resize_mode_(false),
      scrolls_overflow_(false),
      in_overflow_relayout_(false),
      allow_second_overflow_relayout_(false),
      needs_composited_scrolling_(false),
      rebuild_horizontal_scrollbar_layer_(false),
      rebuild_vertical_scrollbar_layer_(false),
      needs_scroll_offset_clamp_(false),
      needs_relayout_(false),
      had_horizontal_scrollbar_before_relayout_(false),
      had_vertical_scrollbar_before_relayout_(false),
      scroll_origin_changed_(false),
      scrollbar_manager_(*this),
      scroll_corner_(nullptr),
      resizer_(nullptr),
      scroll_anchor_(this),
      non_composited_main_thread_scrolling_reasons_(0),
      horizontal_scrollbar_previously_was_overlay_(false),
      vertical_scrollbar_previously_was_overlay_(false),
      scrolling_background_display_item_client_(*this) {
  Node* node = GetLayoutBox()->GetNode();
  if (node && node->IsElementNode()) {
    Element* element = ToElement(node);
    scroll_offset_ = element->SavedLayerScrollOffset();
    if (!scroll_offset_.IsZero())
      GetScrollAnimator().SetCurrentOffset(scroll_offset_);
    element->SetSavedLayerScrollOffset(ScrollOffset());
  }
  UpdateResizerAreaSet();
}
