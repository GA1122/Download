void MetalayerMode::UpdateView() {
  if (!highlight_view_)
    return;

  const base::string16 text = l10n_util::GetStringUTF16(
      loading() ? IDS_ASH_STYLUS_TOOLS_METALAYER_MODE_LOADING
                : IDS_ASH_STYLUS_TOOLS_METALAYER_MODE);
  highlight_view_->text_label()->SetText(text);
  highlight_view_->SetAccessibleName(text);

  highlight_view_->SetEnabled(selectable());

  TrayPopupItemStyle style(TrayPopupItemStyle::FontStyle::DETAILED_VIEW_LABEL);
  style.set_color_style(highlight_view_->enabled()
                            ? TrayPopupItemStyle::ColorStyle::ACTIVE
                            : TrayPopupItemStyle::ColorStyle::DISABLED);

  style.SetupLabel(highlight_view_->text_label());

  highlight_view_->left_icon()->SetImage(
      CreateVectorIcon(GetPaletteIcon(), kMenuIconSize, style.GetIconColor()));
}
