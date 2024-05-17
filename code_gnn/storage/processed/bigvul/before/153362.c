void TabStrip::AddTabAt(int model_index, TabRendererData data, bool is_active) {
  const int view_index =
      (model_index > 0) ? (GetIndexOf(tab_at(model_index - 1)) + 1) : 0;

  Tab* tab = new Tab(this);
  AddChildViewAt(tab, view_index);
  const bool pinned = data.pinned;
  UpdateTabsClosingMap(model_index, 1);
  tabs_.Add(tab, model_index);
  selected_tabs_.IncrementFrom(model_index);

  tab->SetData(std::move(data));

  if (touch_layout_) {
    int add_types = 0;
    if (pinned)
      add_types |= StackedTabStripLayout::kAddTypePinned;
    if (is_active)
      add_types |= StackedTabStripLayout::kAddTypeActive;
    touch_layout_->AddTab(model_index, add_types,
                          UpdateIdealBoundsForPinnedTabs(nullptr));
  }

  if (tab_count() > 1 && GetWidget() && GetWidget()->IsVisible())
    StartInsertTabAnimation(model_index);
  else
    DoLayout();

  SwapLayoutIfNecessary();
  UpdateAccessibleTabIndices();

  for (TabStripObserver& observer : observers_)
    observer.OnTabAdded(model_index);

  if (!drag_context_->IsMutating() && drag_context_->IsDraggingWindow())
    EndDrag(END_DRAG_COMPLETE);
}
