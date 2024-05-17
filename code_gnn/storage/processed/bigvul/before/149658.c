void ScrollAnchor::Dispose() {
  if (scroller_) {
    LocalFrameView* frame_view = ScrollerLayoutBox(scroller_)->GetFrameView();
    auto* root_frame_viewport = DynamicTo<RootFrameViewport>(scroller_.Get());
    ScrollableArea* owning_scroller =
        root_frame_viewport ? &root_frame_viewport->LayoutViewport()
                            : scroller_.Get();
    frame_view->DequeueScrollAnchoringAdjustment(owning_scroller);
    scroller_.Clear();
  }
  anchor_object_ = nullptr;
  saved_selector_ = String();
}
