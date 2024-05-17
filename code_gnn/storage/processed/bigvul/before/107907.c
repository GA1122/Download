void BeforeTranslateInfoBar::UpdateOriginalButtonText() {
  string16 language = GetDelegate()->GetLanguageDisplayableNameAt(
      GetDelegate()->original_language_index());
  language_menu_button_->SetText(UTF16ToWideHack(language));
  language_menu_button_->ClearMaxTextSize();
  Layout();
  SchedulePaint();
}
