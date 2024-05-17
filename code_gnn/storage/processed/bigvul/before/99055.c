void PageInfoWindowView::Init(gfx::NativeWindow parent) {
#if defined(OS_WIN)
  DWORD sys_color = ::GetSysColor(COLOR_3DFACE);
  SkColor color = SkColorSetRGB(GetRValue(sys_color), GetGValue(sys_color),
                                GetBValue(sys_color));
  set_background(views::Background::CreateSolidBackground(color));
#endif

  LayoutSections();

  if (opened_window_count_ > 0) {
    gfx::Rect bounds;
    bool maximized = false;
    if (GetSavedWindowBounds(&bounds) && GetSavedMaximizedState(&maximized)) {
      CalculateWindowBounds(&bounds);
      SaveWindowPlacement(bounds, maximized);
    }
  }

  views::Window::CreateChromeWindow(parent, gfx::Rect(), this);
}
