void PaintLayerScrollableArea::UpdateScrollableAreaSet() {
  LocalFrame* frame = GetLayoutBox()->GetFrame();
  if (!frame)
    return;

  LocalFrameView* frame_view = frame->View();
  if (!frame_view)
    return;

  bool has_overflow =
      !GetLayoutBox()->Size().IsZero() &&
      ((HasHorizontalOverflow() && GetLayoutBox()->ScrollsOverflowX()) ||
       (HasVerticalOverflow() && GetLayoutBox()->ScrollsOverflowY()));

  bool is_visible_to_hit_test =
      GetLayoutBox()->StyleRef().VisibleToHitTesting();
  bool did_scroll_overflow = scrolls_overflow_;
  if (GetLayoutBox()->IsLayoutView()) {
    ScrollbarMode h_mode;
    ScrollbarMode v_mode;
    ToLayoutView(GetLayoutBox())->CalculateScrollbarModes(h_mode, v_mode);
    if (h_mode == kScrollbarAlwaysOff && v_mode == kScrollbarAlwaysOff)
      has_overflow = false;
  }
  scrolls_overflow_ = has_overflow && is_visible_to_hit_test;
  if (did_scroll_overflow == ScrollsOverflow())
    return;

  if (RuntimeEnabledFeatures::ImplicitRootScrollerEnabled() &&
      scrolls_overflow_) {
    if (GetLayoutBox()->IsLayoutView()) {
      if (Element* owner = GetLayoutBox()->GetDocument().LocalOwner()) {
        owner->GetDocument().GetRootScrollerController().ConsiderForImplicit(
            *owner);
      }
    } else {
      GetLayoutBox()
          ->GetDocument()
          .GetRootScrollerController()
          .ConsiderForImplicit(*GetLayoutBox()->GetNode());
    }
  }

  GetLayoutBox()->SetNeedsPaintPropertyUpdate();

  if (scrolls_overflow_) {
    DCHECK(CanHaveOverflowScrollbars(*GetLayoutBox()));
    frame_view->AddScrollableArea(this);
  } else {
    frame_view->RemoveScrollableArea(this);
  }

  layer_->DidUpdateScrollsOverflow();
}
