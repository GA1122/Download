static bool CandidateMayMoveWithScroller(const LayoutObject* candidate,
                                         const ScrollableArea* scroller) {
  if (const ComputedStyle* style = candidate->Style()) {
    if (style->HasViewportConstrainedPosition() ||
        style->HasStickyConstrainedPosition())
      return false;
  }

  LayoutObject::AncestorSkipInfo skip_info(ScrollerLayoutBox(scroller));
  candidate->Container(&skip_info);
  return !skip_info.AncestorSkipped();
}
