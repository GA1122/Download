const gfx::ImageSkia PageInfoUI::GetSiteSettingsIcon(
    const SkColor related_text_color) {
  return gfx::CreateVectorIcon(
      kSettingsIcon, kVectorIconSize,
      color_utils::DeriveDefaultIconColor(related_text_color));
}
