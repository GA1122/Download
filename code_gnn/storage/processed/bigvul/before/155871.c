void ProfileChooserView::CreateAccountButton(views::GridLayout* layout,
                                             const std::string& account_id,
                                             bool is_primary_account,
                                             bool reauth_required,
                                             int width) {
  ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();
  std::string email = signin_ui_util::GetDisplayEmail(browser_->profile(),
                                                      account_id);
  ui::ResourceBundle* rb = &ui::ResourceBundle::GetSharedInstance();
  const gfx::ImageSkia* delete_default_image =
      rb->GetImageNamed(IDR_CLOSE_1).ToImageSkia();
  const int kDeleteButtonWidth = delete_default_image->width();
  gfx::ImageSkia warning_default_image;
  int warning_button_width = 0;
  if (reauth_required) {
    const int kIconSize = 18;
    warning_default_image = gfx::CreateVectorIcon(
        vector_icons::kWarningIcon, kIconSize, gfx::kChromeIconGrey);
    warning_button_width =
        kIconSize +
        provider->GetDistanceMetric(views::DISTANCE_RELATED_BUTTON_HORIZONTAL);
  }

  const gfx::Insets dialog_insets =
      provider->GetInsetsMetric(views::INSETS_DIALOG);

  int available_width =
      width - dialog_insets.width() - kDeleteButtonWidth - warning_button_width;
  HoverButton* email_button =
      new HoverButton(this, warning_default_image, base::UTF8ToUTF16(email));
  email_button->SetEnabled(reauth_required);
  email_button->SetSubtitleElideBehavior(gfx::ELIDE_EMAIL);
  email_button->SetMinSize(gfx::Size(0, kButtonHeight));
  email_button->SetMaxSize(gfx::Size(available_width, kButtonHeight));
  layout->StartRow(1.0, 0);
  layout->AddView(email_button);

  if (reauth_required)
    reauth_account_button_map_[email_button] = account_id;

  if (!browser_->profile()->IsSupervised()) {
    views::ImageButton* delete_button = new views::ImageButton(this);
    delete_button->SetImageAlignment(views::ImageButton::ALIGN_RIGHT,
                                     views::ImageButton::ALIGN_MIDDLE);
    delete_button->SetImage(views::ImageButton::STATE_NORMAL,
                            delete_default_image);
    delete_button->SetImage(views::ImageButton::STATE_HOVERED,
                            rb->GetImageSkiaNamed(IDR_CLOSE_1_H));
    delete_button->SetImage(views::ImageButton::STATE_PRESSED,
                            rb->GetImageSkiaNamed(IDR_CLOSE_1_P));
    delete_button->SetBounds(
        width - provider->GetInsetsMetric(views::INSETS_DIALOG).right() -
            kDeleteButtonWidth,
        views::GridLayout::kFixedSize, kDeleteButtonWidth, kButtonHeight);

    email_button->set_notify_enter_exit_on_child(true);
    email_button->AddChildView(delete_button);

    delete_account_button_map_[delete_button] = account_id;
  }
}
