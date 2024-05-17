static CompositingReasons CompositingReasonsForScroll(const LayoutBox& box) {
  CompositingReasons compositing_reasons = CompositingReason::kNone;
  if (auto* scrollable_area = box.GetScrollableArea()) {
    if (auto* layer = scrollable_area->Layer()) {
      if (CompositingReasonFinder::RequiresCompositingForRootScroller(*layer))
        compositing_reasons |= CompositingReason::kRootScroller;
    }
  }
  return compositing_reasons;
}
