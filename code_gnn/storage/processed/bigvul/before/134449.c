void TabStrip::UpdateDropIndex(const DropTargetEvent& event) {
  const int x = GetMirroredXInView(event.x());
  for (int i = GetMiniTabCount(); i < tab_count(); ++i) {
    Tab* tab = tab_at(i);
    const int tab_max_x = tab->x() + tab->width();
    const int hot_width = tab->width() / kTabEdgeRatioInverse;
    if (x < tab_max_x) {
      if (x < tab->x() + hot_width)
        SetDropIndex(i, true);
      else if (x >= tab_max_x - hot_width)
        SetDropIndex(i + 1, true);
      else
        SetDropIndex(i, false);
      return;
    }
  }

  SetDropIndex(tab_count(), true);
}
