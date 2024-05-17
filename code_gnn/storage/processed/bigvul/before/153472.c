void TabStrip::SetTabData(int model_index, TabRendererData data) {
  Tab* tab = tab_at(model_index);
  const bool pinned_state_changed = tab->data().pinned != data.pinned;
  tab->SetData(std::move(data));

  if (HoverCardIsShowingForTab(tab))
    UpdateHoverCard(tab, true);

  if (pinned_state_changed) {
    if (touch_layout_) {
      int pinned_tab_count = 0;
      int start_x = UpdateIdealBoundsForPinnedTabs(&pinned_tab_count);
      touch_layout_->SetXAndPinnedCount(start_x, pinned_tab_count);
    }
    if (GetWidget() && GetWidget()->IsVisible())
      StartPinnedTabAnimation();
    else
      DoLayout();
  }
  SwapLayoutIfNecessary();
}
