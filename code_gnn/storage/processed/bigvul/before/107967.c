 views::MenuButton* TranslateInfoBarBase::CreateMenuButton(
    const string16& text,
    bool normal_has_border,
    views::ViewMenuDelegate* menu_delegate) {
  views::MenuButton* menu_button =
      new views::MenuButton(NULL, std::wstring(), menu_delegate, true);
  menu_button->set_border(new InfoBarButtonBorder);
  menu_button->set_menu_marker(ResourceBundle::GetSharedInstance().
      GetBitmapNamed(IDR_INFOBARBUTTON_MENU_DROPARROW));
  if (normal_has_border) {
    menu_button->SetNormalHasBorder(true);   
    menu_button->SetAnimationDuration(0);
  }
  menu_button->SetEnabledColor(SK_ColorBLACK);
  menu_button->SetHighlightColor(SK_ColorBLACK);
  menu_button->SetHoverColor(SK_ColorBLACK);

  menu_button->SetFont(ResourceBundle::GetSharedInstance().GetFont(
      ResourceBundle::MediumFont));
  menu_button->SetText(UTF16ToWideHack(text));
  menu_button->ClearMaxTextSize();
  menu_button->SizeToPreferredSize();
  return menu_button;
}
