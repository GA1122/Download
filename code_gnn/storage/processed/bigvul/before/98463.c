AeroPeekWindow::AeroPeekWindow(HWND frame_window,
                               AeroPeekWindowDelegate* delegate,
                               int tab_id,
                               bool tab_active,
                               const std::wstring& title,
                               const SkBitmap& favicon_bitmap)
    : frame_window_(frame_window),
      delegate_(delegate),
      tab_id_(tab_id),
      tab_active_(tab_active),
      ready_to_update_thumbnail_(false, true),
      title_(title),
      favicon_bitmap_(favicon_bitmap),
      frame_icon_(NULL) {
  set_initial_class_style(0);
  set_window_style(WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION);
  set_window_ex_style(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE);
}
