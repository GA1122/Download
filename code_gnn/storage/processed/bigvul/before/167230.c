views::MdTextButton* ConfirmInfoBar::CreateButton(
    ConfirmInfoBarDelegate::InfoBarButton type) {
  auto* button =
      views::MdTextButton::Create(this, GetDelegate()->GetButtonLabel(type));
  button->SetProperty(
      views::kMarginsKey,
      new gfx::Insets(ChromeLayoutProvider::Get()->GetDistanceMetric(
                          DISTANCE_TOAST_CONTROL_VERTICAL),
                      0));
  AddChildView(button);
  button->SizeToPreferredSize();
  return button;
}
