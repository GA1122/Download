bool Browser::IsPopupOrPanel(const TabContents* source) const {
  return is_type_popup() || is_type_panel();
}
