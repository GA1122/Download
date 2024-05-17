void BackingStoreWin::ScrollBackingStore(int dx, int dy,
                                         const gfx::Rect& clip_rect,
                                         const gfx::Size& view_size) {
  RECT damaged_rect, r = clip_rect.ToRECT();
  ScrollDC(hdc_, dx, dy, NULL, &r, NULL, &damaged_rect);

  DCHECK(dx == 0 || dy == 0);
}
