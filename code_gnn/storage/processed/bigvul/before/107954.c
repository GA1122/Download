LinkInfoBar::LinkInfoBar(LinkInfoBarDelegate* delegate)
    : InfoBar(delegate),
      icon_(new views::ImageView),
      label_1_(new views::Label),
      label_2_(new views::Label),
      link_(new views::Link) {
  if (delegate->GetIcon())
    icon_->SetImage(delegate->GetIcon());
  AddChildView(icon_);

  size_t offset;
  string16 message_text = delegate->GetMessageTextWithOffset(&offset);
  if (offset != string16::npos) {
    label_1_->SetText(UTF16ToWideHack(message_text.substr(0, offset)));
    label_2_->SetText(UTF16ToWideHack(message_text.substr(offset)));
  } else {
    label_1_->SetText(UTF16ToWideHack(message_text));
  }
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  label_1_->SetFont(rb.GetFont(ResourceBundle::MediumFont));
  label_2_->SetFont(rb.GetFont(ResourceBundle::MediumFont));
  label_1_->SetColor(SK_ColorBLACK);
  label_2_->SetColor(SK_ColorBLACK);
  label_1_->SetHorizontalAlignment(views::Label::ALIGN_LEFT);
  label_2_->SetHorizontalAlignment(views::Label::ALIGN_LEFT);
  AddChildView(label_1_);
  AddChildView(label_2_);

  link_->SetText(UTF16ToWideHack(delegate->GetLinkText()));
  link_->SetFont(rb.GetFont(ResourceBundle::MediumFont));
  link_->SetHorizontalAlignment(views::Label::ALIGN_LEFT);
  link_->SetController(this);
  link_->MakeReadableOverBackgroundColor(background()->get_color());
  AddChildView(link_);
}
