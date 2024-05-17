int newtab_button_h_offset() {
  static int value = -1;
  if (value == -1) {
    switch (ui::GetDisplayLayout()) {
      case ui::LAYOUT_DESKTOP:
        value = -11;
        break;
      case ui::LAYOUT_TOUCH:
        value = -13;
        break;
      default:
        NOTREACHED();
    }
  }
  return value;
}
