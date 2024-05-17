InfoBar::InfoBar(InfoBarDelegate* delegate)
    : delegate_(delegate),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          close_button_(new views::ImageButton(this))),
      ALLOW_THIS_IN_INITIALIZER_LIST(delete_factory_(this)),
      target_height_(kDefaultTargetHeight) {
  set_parent_owned(false);

  set_background(new InfoBarBackground(delegate->GetInfoBarType()));

  switch (delegate->GetInfoBarType()) {
    case InfoBarDelegate::WARNING_TYPE:
      SetAccessibleName(
          l10n_util::GetStringUTF16(IDS_ACCNAME_INFOBAR_WARNING));
      break;
    case InfoBarDelegate::PAGE_ACTION_TYPE:
      SetAccessibleName(
          l10n_util::GetStringUTF16(IDS_ACCNAME_INFOBAR_PAGE_ACTION));
      break;
    default:
      NOTREACHED();
      break;
  }

  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  close_button_->SetImage(views::CustomButton::BS_NORMAL,
                          rb.GetBitmapNamed(IDR_CLOSE_BAR));
  close_button_->SetImage(views::CustomButton::BS_HOT,
                          rb.GetBitmapNamed(IDR_CLOSE_BAR_H));
  close_button_->SetImage(views::CustomButton::BS_PUSHED,
                          rb.GetBitmapNamed(IDR_CLOSE_BAR_P));
  close_button_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_CLOSE));
  AddChildView(close_button_);

  animation_.reset(new ui::SlideAnimation(this));
  animation_->SetTweenType(ui::Tween::LINEAR);
}
