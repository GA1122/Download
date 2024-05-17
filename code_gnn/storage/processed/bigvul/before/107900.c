void AfterTranslateInfoBar::UpdateLanguageButtonText(
    LanguagesMenuModel::LanguageType language_type) {
  int language_index;
  views::MenuButton* language_button;
  if (language_type == LanguagesMenuModel::ORIGINAL) {
    language_index = GetDelegate()->original_language_index();
    language_button = original_language_menu_button_;
  } else {
    language_index = GetDelegate()->target_language_index();
    language_button = target_language_menu_button_;
  }
  string16 language =
      GetDelegate()->GetLanguageDisplayableNameAt(language_index);
  language_button->SetText(UTF16ToWideHack(language));
  language_button->ClearMaxTextSize();
  Layout();
  SchedulePaint();
}
