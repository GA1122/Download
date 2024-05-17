void TabStrip::MoveTab(int from_model_index,
                       int to_model_index,
                       TabRendererData data) {
  DCHECK_GT(tabs_.view_size(), 0);

  const Tab* last_tab = GetLastVisibleTab();
  tab_at(from_model_index)->SetData(std::move(data));

  const int to_view_index = GetIndexOf(tab_at(to_model_index));
  ReorderChildView(tab_at(from_model_index), to_view_index);

  if (touch_layout_) {
    tabs_.MoveViewOnly(from_model_index, to_model_index);
    int pinned_count = 0;
    const int start_x = UpdateIdealBoundsForPinnedTabs(&pinned_count);
    touch_layout_->MoveTab(from_model_index, to_model_index,
                           controller_->GetActiveIndex(), start_x,
                           pinned_count);
  } else {
    tabs_.Move(from_model_index, to_model_index);
  }
  selected_tabs_.Move(from_model_index, to_model_index,  1);

  StartMoveTabAnimation();
  if (TabDragController::IsAttachedTo(GetDragContext()) &&
      (last_tab != GetLastVisibleTab() || last_tab->dragging())) {
    new_tab_button_->SetVisible(false);
  }
  SwapLayoutIfNecessary();

  UpdateAccessibleTabIndices();

  for (TabStripObserver& observer : observers_)
    observer.OnTabMoved(from_model_index, to_model_index);
}
