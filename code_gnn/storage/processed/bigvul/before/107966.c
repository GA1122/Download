 views::Label* TranslateInfoBarBase::CreateLabel(const string16& text) {
  views::Label* label = new views::Label(UTF16ToWideHack(text),
      ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::MediumFont));
  label->SetColor(SK_ColorBLACK);
  label->SetHorizontalAlignment(views::Label::ALIGN_LEFT);
  return label;
}
