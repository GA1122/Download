void TabStrip::PaintChildren(gfx::Canvas* canvas) {
  Tab* active_tab = NULL;
  std::vector<Tab*> tabs_dragging;
  std::vector<Tab*> selected_tabs;
  int selected_tab_count = 0;
  bool is_dragging = false;
  int active_tab_index = -1;
  const bool stacking = (layout_type_ == TAB_STRIP_LAYOUT_STACKED) ||
      IsStackingDraggedTabs();

  const chrome::HostDesktopType host_desktop_type =
      chrome::GetHostDesktopTypeForNativeView(GetWidget()->GetNativeView());
  const int inactive_tab_alpha =
      host_desktop_type == chrome::HOST_DESKTOP_TYPE_ASH ?
      kInactiveTabAndNewTabButtonAlphaAsh :
      kInactiveTabAndNewTabButtonAlpha;

  if (inactive_tab_alpha < 255)
    canvas->SaveLayerAlpha(inactive_tab_alpha);

  PaintClosingTabs(canvas, tab_count());

  for (int i = tab_count() - 1; i >= 0; --i) {
    Tab* tab = tab_at(i);
    if (tab->IsSelected())
      selected_tab_count++;
    if (tab->dragging() && !stacking) {
      is_dragging = true;
      if (tab->IsActive()) {
        active_tab = tab;
        active_tab_index = i;
      } else {
        tabs_dragging.push_back(tab);
      }
    } else if (!tab->IsActive()) {
      if (!tab->IsSelected()) {
        if (!stacking)
          tab->Paint(canvas);
      } else {
        selected_tabs.push_back(tab);
      }
    } else {
      active_tab = tab;
      active_tab_index = i;
    }
    PaintClosingTabs(canvas, i);
  }

  if (stacking && active_tab_index >= 0) {
    for (int i = 0; i < active_tab_index; ++i) {
      Tab* tab = tab_at(i);
      tab->Paint(canvas);
    }

    for (int i = tab_count() - 1; i > active_tab_index; --i) {
      Tab* tab = tab_at(i);
      tab->Paint(canvas);
    }
  }
  if (inactive_tab_alpha < 255)
    canvas->Restore();

  if (GetWidget()->ShouldUseNativeFrame()) {
    SkPaint paint;
    int alpha = selected_tab_count > 1 ?
        kNativeFrameInactiveTabAlphaMultiSelection :
        kNativeFrameInactiveTabAlpha;
    paint.setColor(SkColorSetARGB(alpha, 255, 255, 255));
    paint.setXfermodeMode(SkXfermode::kDstIn_Mode);
    paint.setStyle(SkPaint::kFill_Style);
    canvas->DrawRect(gfx::Rect(0, 0, width(), height() - 2), paint);
  }

  for (size_t i = 0; i < selected_tabs.size(); ++i)
    selected_tabs[i]->Paint(canvas);

  if (active_tab && !is_dragging)
    active_tab->Paint(canvas);

  if (inactive_tab_alpha < 255)
    canvas->SaveLayerAlpha(inactive_tab_alpha);
  newtab_button_->Paint(canvas);
  if (inactive_tab_alpha < 255)
    canvas->Restore();

  for (size_t i = 0; i < tabs_dragging.size(); ++i)
    tabs_dragging[i]->Paint(canvas);

  if (active_tab && is_dragging)
    active_tab->Paint(canvas);
}
