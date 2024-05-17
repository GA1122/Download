int newtab_button_asset_width() {
  static int value = -1;
  if (value == -1) {
    switch (ui::GetDisplayLayout()) {
      case ui::LAYOUT_DESKTOP:
        value = 34;
        break;
      case ui::LAYOUT_TOUCH:
        value = 46;
        break;
      default:
        NOTREACHED();
    }
  }
  return value;
}
