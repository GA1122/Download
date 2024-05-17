void TabStrip::RemoveTabAt(content::WebContents* contents,
                           int model_index,
                           bool was_active) {
  const int model_count = GetModelCount();
  const int tab_overlap = TabStyle::GetTabOverlap();
  if (in_tab_close_ && model_count > 0 && model_index != model_count) {
    int next_active_index = controller_->GetActiveIndex();
    DCHECK(IsValidModelIndex(next_active_index));
    if (model_index <= next_active_index) {
      next_active_index++;
    }
    Tab* next_active_tab = tab_at(next_active_index);
    Tab* tab_being_removed = tab_at(model_index);

    int size_delta = tab_being_removed->width();
    if (!tab_being_removed->data().pinned && was_active &&
        GetActiveTabWidth() > GetInactiveTabWidth()) {
      size_delta = next_active_tab->width();
    }

    available_width_for_tabs_ =
        ideal_bounds(model_count).right() - size_delta + tab_overlap;
  }

  if (!touch_layout_)
    PrepareForAnimation();

  Tab* tab = tab_at(model_index);
  tab->SetClosing(true);

  int old_x = tabs_.ideal_bounds(model_index).x();
  RemoveTabFromViewModel(model_index);

  if (touch_layout_) {
    touch_layout_->RemoveTab(model_index,
                             UpdateIdealBoundsForPinnedTabs(nullptr), old_x);
  }

  UpdateIdealBounds();
  AnimateToIdealBounds();


  gfx::Rect tab_bounds = tab->bounds();
  tab_bounds.set_x((model_index > 0)
                       ? (ideal_bounds(model_index - 1).right() - tab_overlap)
                       : 0);

  tab_bounds.set_width(tab_overlap);

  bounds_animator_.AnimateViewTo(
      tab, tab_bounds, std::make_unique<RemoveTabDelegate>(this, tab));

  if ((touch_layout_ || !in_tab_close_ || model_index == GetModelCount()) &&
      TabDragController::IsAttachedTo(GetDragContext())) {
    bounds_animator_.StopAnimatingView(new_tab_button_);
    new_tab_button_->SetBoundsRect(new_tab_button_bounds_);
  }

  SwapLayoutIfNecessary();

  UpdateAccessibleTabIndices();

  UpdateHoverCard(nullptr, false);

  for (TabStripObserver& observer : observers_)
    observer.OnTabRemoved(model_index);

  if (!drag_context_->IsMutating() && drag_context_->IsDraggingTab(contents))
    EndDrag(END_DRAG_COMPLETE);
}
