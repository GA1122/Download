void ScrollAnchor::NotifyBeforeLayout() {
  if (queued_) {
    scroll_anchor_disabling_style_changed_ |=
        ComputeScrollAnchorDisablingStyleChanged();
    return;
  }
  DCHECK(scroller_);
  ScrollOffset scroll_offset = scroller_->GetScrollOffset();
  float block_direction_scroll_offset =
      ScrollerLayoutBox(scroller_)->IsHorizontalWritingMode()
          ? scroll_offset.Height()
          : scroll_offset.Width();
  if (block_direction_scroll_offset == 0) {
    ClearSelf();
    return;
  }

  if (!anchor_object_) {
    FindAnchor();
    if (!anchor_object_)
      return;
  }

  scroll_anchor_disabling_style_changed_ =
      ComputeScrollAnchorDisablingStyleChanged();

  LocalFrameView* frame_view = ScrollerLayoutBox(scroller_)->GetFrameView();
  auto* root_frame_viewport = DynamicTo<RootFrameViewport>(scroller_.Get());
  ScrollableArea* owning_scroller = root_frame_viewport
                                        ? &root_frame_viewport->LayoutViewport()
                                        : scroller_.Get();
  frame_view->EnqueueScrollAnchoringAdjustment(owning_scroller);
  queued_ = true;
}
