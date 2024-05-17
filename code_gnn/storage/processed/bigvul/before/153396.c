BrowserRootView::DropIndex TabStrip::GetDropIndex(
    const ui::DropTargetEvent& event) {
  StopAnimating(true);

  const int x = GetMirroredXInView(event.x());
  for (int i = 0; i < tab_count(); ++i) {
    Tab* tab = tab_at(i);
    const int tab_max_x = tab->x() + tab->width();

    const int hot_width = tab->width() / 4;

    if (x < tab_max_x) {
      if (x >= (tab_max_x - hot_width))
        return {i + 1, true};
      return {i, x < tab->x() + hot_width};
    }
  }

  return {tab_count(), true};
}
