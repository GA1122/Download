void TabStrip::UpdateAccessibleTabIndices() {
  const int num_tabs = tab_count();
  for (int i = 0; i < num_tabs; ++i)
    tab_at(i)->GetViewAccessibility().OverridePosInSet(i + 1, num_tabs);
}
