void TabStrip::AddTabAt(int model_index,
                        const TabRendererData& data,
                        bool is_active) {
  if (drag_controller_.get() && !drag_controller_->is_mutating() &&
      drag_controller_->is_dragging_window()) {
    EndDrag(END_DRAG_COMPLETE);
  }
  Tab* tab = CreateTab();
  tab->SetData(data);
  UpdateTabsClosingMap(model_index, 1);
  tabs_.Add(tab, model_index);
  AddChildView(tab);

  if (touch_layout_.get()) {
    GenerateIdealBoundsForMiniTabs(NULL);
    int add_types = 0;
    if (data.mini)
      add_types |= StackedTabStripLayout::kAddTypeMini;
    if (is_active)
      add_types |= StackedTabStripLayout::kAddTypeActive;
    touch_layout_->AddTab(model_index, add_types, GetStartXForNormalTabs());
  }

  if (tab_count() > 1 && GetWidget() && GetWidget()->IsVisible())
    StartInsertTabAnimation(model_index);
  else
    DoLayout();

  SwapLayoutIfNecessary();

  FOR_EACH_OBSERVER(TabStripObserver, observers_,
                    TabStripAddedTabAt(this, model_index));
}
