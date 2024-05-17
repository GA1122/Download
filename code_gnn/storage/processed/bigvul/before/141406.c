void PaintLayerScrollableArea::Dispose() {
  if (InResizeMode() && !GetLayoutBox()->DocumentBeingDestroyed()) {
    if (LocalFrame* frame = GetLayoutBox()->GetFrame())
      frame->GetEventHandler().ResizeScrollableAreaDestroyed();
  }

  if (LocalFrame* frame = GetLayoutBox()->GetFrame()) {
    if (LocalFrameView* frame_view = frame->View()) {
      frame_view->RemoveScrollableArea(this);
      frame_view->RemoveAnimatingScrollableArea(this);
    }
  }

  non_composited_main_thread_scrolling_reasons_ = 0;

  if (ScrollingCoordinator* scrolling_coordinator = GetScrollingCoordinator())
    scrolling_coordinator->WillDestroyScrollableArea(this);

  if (!GetLayoutBox()->DocumentBeingDestroyed()) {
    Node* node = GetLayoutBox()->GetNode();
    if (node && node->IsElementNode())
      ToElement(node)->SetSavedLayerScrollOffset(scroll_offset_);
  }

  if (LocalFrame* frame = GetLayoutBox()->GetFrame()) {
    if (LocalFrameView* frame_view = frame->View())
      frame_view->RemoveResizerArea(*GetLayoutBox());
  }

  scroll_anchor_.Dispose();

  GetLayoutBox()
      ->GetDocument()
      .GetPage()
      ->GlobalRootScrollerController()
      .DidDisposeScrollableArea(*this);

  scrollbar_manager_.Dispose();

  if (scroll_corner_)
    scroll_corner_->Destroy();
  if (resizer_)
    resizer_->Destroy();

  ClearScrollableArea();

  if (SmoothScrollSequencer* sequencer = GetSmoothScrollSequencer())
    sequencer->DidDisposeScrollableArea(*this);

  layer_ = nullptr;
}
