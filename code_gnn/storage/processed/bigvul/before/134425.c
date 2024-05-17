void TabStrip::SetTabData(int model_index, const TabRendererData& data) {
  Tab* tab = tab_at(model_index);
  bool mini_state_changed = tab->data().mini != data.mini;
  tab->SetData(data);

  if (mini_state_changed) {
    if (touch_layout_.get()) {
      int mini_tab_count = 0;
      int start_x = GenerateIdealBoundsForMiniTabs(&mini_tab_count);
      touch_layout_->SetXAndMiniCount(start_x, mini_tab_count);
    }
    if (GetWidget() && GetWidget()->IsVisible())
      StartMiniTabAnimation();
    else
      DoLayout();
  }
  SwapLayoutIfNecessary();
}
