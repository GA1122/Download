bool Browser::IsPopupOrPanel(const WebContents* source) const {
  return is_type_popup() || is_type_panel();
}
