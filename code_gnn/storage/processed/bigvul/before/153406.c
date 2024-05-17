int GetStackableTabWidth() {
  return TabStyle::GetTabOverlap() + (MD::touch_ui() ? 136 : 102);
}
