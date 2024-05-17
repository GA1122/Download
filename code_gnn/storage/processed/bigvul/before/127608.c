void HideHostCursor() {
  CR_DEFINE_STATIC_LOCAL(XScopedCursor, invisible_cursor,
                         (CreateInvisibleCursor(), ui::GetXDisplay()));
  XDefineCursor(ui::GetXDisplay(), DefaultRootWindow(ui::GetXDisplay()),
                invisible_cursor.get());
}
