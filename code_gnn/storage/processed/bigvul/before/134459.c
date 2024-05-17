int newtab_button_v_offset() {
  static int value = -1;
  if (value == -1) {
    switch (ui::GetDisplayLayout()) {
      case ui::LAYOUT_DESKTOP:
        value = 7;
        break;
      case ui::LAYOUT_TOUCH:
        value = 8;
        break;
      default:
        NOTREACHED();
    }
  }
  return value;
}
