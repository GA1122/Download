void TabStrip::UpdateNewTabButtonBorder() {
  const int extra_vertical_space = GetLayoutConstant(TAB_HEIGHT) -
                                   GetLayoutConstant(TABSTRIP_TOOLBAR_OVERLAP) -
                                   NewTabButton::kButtonSize.height();
  constexpr int kHorizontalInset = 8;
  new_tab_button_->SetBorder(views::CreateEmptyBorder(gfx::Insets(
      extra_vertical_space / 2, kHorizontalInset, 0, kHorizontalInset)));
}
