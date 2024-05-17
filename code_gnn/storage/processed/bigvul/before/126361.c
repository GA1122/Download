GdkRegion* BrowserWindowGtk::GetWindowShape(int width, int height) const {
  if (UseCustomFrame() && !IsFullscreen() && !IsMaximized()) {
    GdkRectangle top_top_rect = { 3, 0, width - 6, 1 };
    GdkRectangle top_mid_rect = { 1, 1, width - 2, 2 };
    GdkRectangle mid_rect = { 0, 3, width, height - 6 };
    GdkRectangle bot_mid_rect = top_mid_rect;
    bot_mid_rect.y = height - 3;
    GdkRectangle bot_bot_rect = top_top_rect;
    bot_bot_rect.y = height - 1;
    GdkRegion* mask = gdk_region_rectangle(&top_top_rect);
    gdk_region_union_with_rect(mask, &top_mid_rect);
    gdk_region_union_with_rect(mask, &mid_rect);
    gdk_region_union_with_rect(mask, &bot_mid_rect);
    gdk_region_union_with_rect(mask, &bot_bot_rect);
    return mask;
  } else if (UseCustomFrame()) {
    GdkRectangle rect = { 0, 0, width, height };
    GdkRegion* mask = gdk_region_rectangle(&rect);
    return mask;
  } else {
    return NULL;
  }
}
