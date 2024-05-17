PaintLayerScrollableArea::PreventRelayoutScope::~PreventRelayoutScope() {
  if (--count_ == 0) {
    if (relayout_needed_) {
      for (auto scrollable_area : NeedsRelayoutList()) {
        DCHECK(scrollable_area->NeedsRelayout());
        LayoutBox* box = scrollable_area->GetLayoutBox();
        layout_scope_->SetNeedsLayout(
            box, layout_invalidation_reason::kScrollbarChanged);
        if (box->IsLayoutBlock()) {
          bool horizontal_scrollbar_changed =
              scrollable_area->HasHorizontalScrollbar() !=
              scrollable_area->HadHorizontalScrollbarBeforeRelayout();
          bool vertical_scrollbar_changed =
              scrollable_area->HasVerticalScrollbar() !=
              scrollable_area->HadVerticalScrollbarBeforeRelayout();
          if (horizontal_scrollbar_changed || vertical_scrollbar_changed) {
            ToLayoutBlock(box)->ScrollbarsChanged(horizontal_scrollbar_changed,
                                                  vertical_scrollbar_changed);
          }
        }
        scrollable_area->SetNeedsRelayout(false);
      }

      NeedsRelayoutList().clear();
    }
    layout_scope_ = nullptr;
  }
}
