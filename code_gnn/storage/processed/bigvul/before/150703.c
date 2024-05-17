const gfx::ImageSkia PageInfoUI::GetVrSettingsIcon(SkColor related_text_color) {
  return gfx::CreateVectorIcon(
      kVrHeadsetIcon, kVectorIconSize,
      color_utils::DeriveDefaultIconColor(related_text_color));
}
