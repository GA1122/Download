SaveCCInfoConfirmInfoBar::SaveCCInfoConfirmInfoBar(
    ConfirmInfoBarDelegate* delegate)
    : AlertInfoBar(delegate),
      initialized_(false) {
  save_button_ = InfoBarTextButton::Create(this,
      delegate->GetButtonLabel(ConfirmInfoBarDelegate::BUTTON_OK));
  dont_save_button_ = InfoBarTextButton::Create(this,
      delegate->GetButtonLabel(ConfirmInfoBarDelegate::BUTTON_CANCEL));

  link_ = new views::Link;
  link_->SetText(delegate->GetLinkText());
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  link_->SetFont(rb.GetFont(ResourceBundle::MediumFont));
  link_->SetHorizontalAlignment(views::Label::ALIGN_LEFT);
  link_->SetController(this);
  link_->MakeReadableOverBackgroundColor(background()->get_color());
}
