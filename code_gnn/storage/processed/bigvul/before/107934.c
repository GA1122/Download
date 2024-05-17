ConfirmInfoBar::ConfirmInfoBar(ConfirmInfoBarDelegate* delegate)
    : AlertInfoBar(delegate),
      ok_button_(NULL),
      cancel_button_(NULL),
      link_(NULL),
      initialized_(false) {
  ok_button_ = InfoBarTextButton::Create(this,
      (delegate->GetButtons() & ConfirmInfoBarDelegate::BUTTON_OK) ?
          delegate->GetButtonLabel(ConfirmInfoBarDelegate::BUTTON_OK) :
          string16());
  ok_button_->SetAccessibleName(WideToUTF16Hack(ok_button_->text()));
  cancel_button_ = InfoBarTextButton::Create(this,
      (delegate->GetButtons() & ConfirmInfoBarDelegate::BUTTON_CANCEL) ?
          delegate->GetButtonLabel(ConfirmInfoBarDelegate::BUTTON_CANCEL) :
          string16());
  cancel_button_->SetAccessibleName(WideToUTF16Hack(cancel_button_->text()));

  link_ = new views::Link;
  link_->SetText(UTF16ToWideHack(delegate->GetLinkText()));
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  link_->SetFont(rb.GetFont(ResourceBundle::MediumFont));
  link_->SetHorizontalAlignment(views::Label::ALIGN_LEFT);
  link_->SetController(this);
  link_->MakeReadableOverBackgroundColor(background()->get_color());
}
