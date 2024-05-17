int TabStyleViews::GetMinimumInactiveWidth() {
  constexpr int kInteriorWidth = 16;
  return kInteriorWidth - GetSeparatorSize().width() + GetTabOverlap();
}
