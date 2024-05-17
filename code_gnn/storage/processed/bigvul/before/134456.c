int newtab_button_asset_height() {
  static int value = -1;
  if (value == -1) {
    switch (ui::GetDisplayLayout()) {
      case ui::LAYOUT_DESKTOP:
        value = 18;
        break;
      case ui::LAYOUT_TOUCH:
        value = 24;
        break;
      default:
        NOTREACHED();
    }
  }
  return value;
}
