views::View* ProfileChooserView::CreateAutofillHomeView() {
  if (browser_->profile()->IsGuestSession())
    return nullptr;

  views::View* view = new views::View();
  view->SetLayoutManager(
      std::make_unique<views::BoxLayout>(views::BoxLayout::kVertical));

  passwords_button_ = new HoverButton(
      this, gfx::CreateVectorIcon(kKeyIcon, kIconSize, gfx::kChromeIconGrey),
      l10n_util::GetStringUTF16(IDS_PROFILES_PASSWORDS_LINK));
  view->AddChildView(passwords_button_);

  credit_cards_button_ = new HoverButton(
      this,
      gfx::CreateVectorIcon(kCreditCardIcon, kIconSize, gfx::kChromeIconGrey),
      l10n_util::GetStringUTF16(IDS_PROFILES_CREDIT_CARDS_LINK));
  view->AddChildView(credit_cards_button_);

  addresses_button_ =
      new HoverButton(this,
                      gfx::CreateVectorIcon(vector_icons::kLocationOnIcon,
                                            kIconSize, gfx::kChromeIconGrey),
                      l10n_util::GetStringUTF16(IDS_PROFILES_ADDRESSES_LINK));
  view->AddChildView(addresses_button_);
  return view;
}
