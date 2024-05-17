AlertInfoBar::AlertInfoBar(ConfirmInfoBarDelegate* delegate)
    : InfoBar(delegate) {
  label_ = new views::Label(
      UTF16ToWideHack(delegate->GetMessageText()),
      ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::MediumFont));
  label_->SetColor(SK_ColorBLACK);
  label_->SetHorizontalAlignment(views::Label::ALIGN_LEFT);
  AddChildView(label_);

  icon_ = new views::ImageView;
  if (delegate->GetIcon())
    icon_->SetImage(delegate->GetIcon());
  AddChildView(icon_);
}
