void TabStrip::Init() {
  SetID(VIEW_ID_TAB_STRIP);
  set_notify_enter_exit_on_child(true);

  new_tab_button_ = new NewTabButton(this, this);
  new_tab_button_->SetTooltipText(
      l10n_util::GetStringUTF16(IDS_TOOLTIP_NEW_TAB));
  new_tab_button_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_NEWTAB));
  new_tab_button_->SetImageVerticalAlignment(views::ImageButton::ALIGN_BOTTOM);
  new_tab_button_->SetEventTargeter(
      std::make_unique<views::ViewTargeter>(new_tab_button_));
  AddChildView(new_tab_button_);

  UpdateNewTabButtonBorder();
  new_tab_button_bounds_.set_size(new_tab_button_->GetPreferredSize());

  if (g_drop_indicator_width == 0) {
    gfx::ImageSkia* drop_image = GetDropArrowImage(true);
    g_drop_indicator_width = drop_image->width();
    g_drop_indicator_height = drop_image->height();
  }

  UpdateContrastRatioValues();

  if (!gfx::Animation::ShouldRenderRichAnimation())
    bounds_animator_.SetAnimationDuration(0);
}
