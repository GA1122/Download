void Textfield::EnsureCaretNotInRect(const gfx::Rect& rect_in_screen) {
#if defined(OS_CHROMEOS)
  aura::Window* top_level_window = GetNativeView()->GetToplevelWindow();
  wm::EnsureWindowNotInRect(top_level_window, rect_in_screen);
#endif   
}
