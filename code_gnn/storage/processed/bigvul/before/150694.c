const gfx::ImageSkia PageInfoUI::GetCertificateIcon(
    const SkColor related_text_color) {
  return gfx::CreateVectorIcon(
      kCertificateIcon, kVectorIconSize,
      color_utils::DeriveDefaultIconColor(related_text_color));
}
