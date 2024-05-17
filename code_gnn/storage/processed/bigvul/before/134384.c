void TabStrip::MoveTab(int from_model_index,
                       int to_model_index,
                       const TabRendererData& data) {
  DCHECK_GT(tabs_.view_size(), 0);
  Tab* last_tab = tab_at(tab_count() - 1);
  tab_at(from_model_index)->SetData(data);
  if (touch_layout_.get()) {
    tabs_.MoveViewOnly(from_model_index, to_model_index);
    int mini_count = 0;
    GenerateIdealBoundsForMiniTabs(&mini_count);
    touch_layout_->MoveTab(
        from_model_index, to_model_index, controller_->GetActiveIndex(),
        GetStartXForNormalTabs(), mini_count);
  } else {
    tabs_.Move(from_model_index, to_model_index);
  }
  StartMoveTabAnimation();
  if (TabDragController::IsAttachedTo(this) &&
      (last_tab != tab_at(tab_count() - 1) || last_tab->dragging())) {
    newtab_button_->SetVisible(false);
  }
  SwapLayoutIfNecessary();

  FOR_EACH_OBSERVER(TabStripObserver, observers_,
                    TabStripMovedTab(this, from_model_index, to_model_index));
}
