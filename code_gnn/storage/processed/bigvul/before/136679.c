void FrameLoader::RestoreScrollPositionAndViewState(
    FrameLoadType load_type,
    HistoryLoadType history_load_type,
    HistoryItem::ViewState* view_state,
    HistoryScrollRestorationType scroll_restoration_type) {
  LocalFrameView* view = frame_->View();
  if (!view || !view->LayoutViewportScrollableArea() ||
      !state_machine_.CommittedFirstRealDocumentLoad() ||
      !frame_->IsAttached()) {
    return;
  }
  if (!NeedsHistoryItemRestore(load_type) || !view_state)
    return;

  bool should_restore_scroll =
      scroll_restoration_type != kScrollRestorationManual;
  bool should_restore_scale = view_state->page_scale_factor_;

  bool can_restore_without_clamping =
      view->LayoutViewportScrollableArea()->ClampScrollOffset(
          view_state->scroll_offset_) == view_state->scroll_offset_;

  bool should_force_clamping =
      !frame_->IsLoading() || history_load_type == kHistorySameDocumentLoad;
  if (!can_restore_without_clamping && should_force_clamping)
    frame_->GetDocument()->UpdateStyleAndLayout();

  bool can_restore_without_annoying_user =
      !GetDocumentLoader()->GetInitialScrollState().was_scrolled_by_user &&
      (can_restore_without_clamping || should_force_clamping ||
       !should_restore_scroll);
  if (!can_restore_without_annoying_user)
    return;

  if (should_restore_scroll) {
    bool did_restore =
        ShouldSerializeScrollAnchor() &&
        view->RestoreScrollAnchor(
            {view_state->scroll_anchor_data_.selector_,
             LayoutPoint(view_state->scroll_anchor_data_.offset_.x,
                         view_state->scroll_anchor_data_.offset_.y),
             view_state->scroll_anchor_data_.simhash_});
    if (!did_restore) {
      view->LayoutViewportScrollableArea()->SetScrollOffset(
          view_state->scroll_offset_, kProgrammaticScroll);
    }
  }

  if (frame_->IsMainFrame()) {
    ScrollOffset visual_viewport_offset(
        view_state->visual_viewport_scroll_offset_);

    if (visual_viewport_offset.Width() == -1 &&
        visual_viewport_offset.Height() == -1) {
      visual_viewport_offset =
          view_state->scroll_offset_ -
          view->LayoutViewportScrollableArea()->GetScrollOffset();
    }

    VisualViewport& visual_viewport = frame_->GetPage()->GetVisualViewport();
    if (should_restore_scale && should_restore_scroll) {
      visual_viewport.SetScaleAndLocation(view_state->page_scale_factor_,
                                          FloatPoint(visual_viewport_offset));
    } else if (should_restore_scale) {
      visual_viewport.SetScale(view_state->page_scale_factor_);
    } else if (should_restore_scroll) {
      visual_viewport.SetLocation(FloatPoint(visual_viewport_offset));
    }

    if (ScrollingCoordinator* scrolling_coordinator =
            frame_->GetPage()->GetScrollingCoordinator())
      scrolling_coordinator->FrameViewRootLayerDidChange(view);
  }

  GetDocumentLoader()->GetInitialScrollState().did_restore_from_history = true;
}
