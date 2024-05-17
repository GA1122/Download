bool IsSelectionModifierDown(const ui::MouseEvent& event) {
#if defined(OS_MACOSX)
  return event.IsCommandDown();
#else
  return event.IsControlDown();
#endif
}
