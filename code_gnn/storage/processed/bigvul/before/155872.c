views::View* ProfileChooserView::CreateAccountRemovalView() {
  ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();

  const gfx::Insets dialog_insets =
      provider->GetInsetsMetric(views::INSETS_DIALOG);

  views::View* view = new views::View();
  views::GridLayout* layout = CreateSingleColumnLayout(
      view, kFixedAccountRemovalViewWidth - dialog_insets.width());

  view->SetBorder(
      views::CreateEmptyBorder(0, dialog_insets.left(),
                               dialog_insets.bottom(), dialog_insets.right()));

  const std::string& primary_account = SigninManagerFactory::GetForProfile(
      browser_->profile())->GetAuthenticatedAccountId();
  bool is_primary_account = primary_account == account_id_to_remove_;

  const int unrelated_vertical_spacing =
      provider->GetDistanceMetric(views::DISTANCE_UNRELATED_CONTROL_VERTICAL);

  layout->StartRowWithPadding(1.0, views::GridLayout::kFixedSize,
                              views::GridLayout::kFixedSize,
                              unrelated_vertical_spacing);

  if (is_primary_account) {
    std::string email = signin_ui_util::GetDisplayEmail(browser_->profile(),
                                                        account_id_to_remove_);
    std::vector<size_t> offsets;
    const base::string16 settings_text =
        l10n_util::GetStringUTF16(IDS_PROFILES_SETTINGS_LINK);
    const base::string16 primary_account_removal_text =
        l10n_util::GetStringFUTF16(IDS_PROFILES_PRIMARY_ACCOUNT_REMOVAL_TEXT,
            base::UTF8ToUTF16(email), settings_text, &offsets);
    views::StyledLabel* primary_account_removal_label =
        new views::StyledLabel(primary_account_removal_text, this);
    primary_account_removal_label->AddStyleRange(
        gfx::Range(offsets[1], offsets[1] + settings_text.size()),
        views::StyledLabel::RangeStyleInfo::CreateForLink());
    primary_account_removal_label->SetTextContext(CONTEXT_BODY_TEXT_SMALL);
    layout->AddView(primary_account_removal_label);
  } else {
    views::Label* content_label = new views::Label(
        l10n_util::GetStringUTF16(IDS_PROFILES_ACCOUNT_REMOVAL_TEXT),
        CONTEXT_BODY_TEXT_SMALL);
    content_label->SetMultiLine(true);
    content_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    layout->AddView(content_label);
  }

  if (!is_primary_account) {
    remove_account_button_ = views::MdTextButton::CreateSecondaryUiBlueButton(
        this, l10n_util::GetStringUTF16(IDS_PROFILES_ACCOUNT_REMOVAL_BUTTON));
    remove_account_button_->SetHorizontalAlignment(
        gfx::ALIGN_CENTER);
    layout->StartRowWithPadding(1.0, views::GridLayout::kFixedSize,
                                views::GridLayout::kFixedSize,
                                unrelated_vertical_spacing);
    layout->AddView(remove_account_button_);
  } else {
    layout->AddPaddingRow(views::GridLayout::kFixedSize,
                          unrelated_vertical_spacing);
  }

  TitleCard* title_card = new TitleCard(
      l10n_util::GetStringUTF16(IDS_PROFILES_ACCOUNT_REMOVAL_TITLE),
      this, &account_removal_cancel_button_);
  return TitleCard::AddPaddedTitleCard(view, title_card,
      kFixedAccountRemovalViewWidth);
}
