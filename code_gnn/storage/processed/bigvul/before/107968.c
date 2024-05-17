gfx::Point TranslateInfoBarBase::DetermineMenuPosition(
    views::MenuButton* menu_button) {
  gfx::Rect lb = menu_button->GetLocalBounds(true);
  gfx::Point menu_position(lb.origin());
  menu_position.Offset(2, lb.height() - 3);
  if (base::i18n::IsRTL())
    menu_position.Offset(lb.width() - 4, 0);

  View::ConvertPointToScreen(menu_button, &menu_position);
#if defined(OS_WIN)
  int left_bound = GetSystemMetrics(SM_XVIRTUALSCREEN);
  if (menu_position.x() < left_bound)
    menu_position.set_x(left_bound);
#endif
  return menu_position;
}
