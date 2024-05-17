void TabStrip::Init() {
  set_id(VIEW_ID_TAB_STRIP);
  set_notify_enter_exit_on_child(true);
  newtab_button_bounds_.SetRect(0,
                                0,
                                newtab_button_asset_width(),
                                newtab_button_asset_height() +
                                    newtab_button_v_offset());
  newtab_button_ = new NewTabButton(this, this);
  newtab_button_->SetTooltipText(
      l10n_util::GetStringUTF16(IDS_TOOLTIP_NEW_TAB));
  newtab_button_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_NEWTAB));
  newtab_button_->SetImageAlignment(views::ImageButton::ALIGN_LEFT,
                                    views::ImageButton::ALIGN_BOTTOM);
  AddChildView(newtab_button_);
  if (drop_indicator_width == 0) {
    gfx::ImageSkia* drop_image = GetDropArrowImage(true);
    drop_indicator_width = drop_image->width();
    drop_indicator_height = drop_image->height();
  }
}
