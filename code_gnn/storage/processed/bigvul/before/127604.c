::Cursor GetXCursor(int cursor_shape) {
  CR_DEFINE_STATIC_LOCAL(XCursorCache, cache, ());

  if (cursor_shape == kCursorClearXCursorCache) {
    cache.Clear();
    return 0;
  }

  return cache.GetCursor(cursor_shape);
}
