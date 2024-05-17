const gfx::ImageSkia PageInfoUI::GetChosenObjectIcon(
    const ChosenObjectInfo& object,
    bool deleted,
    SkColor related_text_color) {
  const gfx::VectorIcon* icon = &gfx::kNoneIcon;
  switch (object.ui_info.content_settings_type) {
    case CONTENT_SETTINGS_TYPE_USB_CHOOSER_DATA:
      icon = &vector_icons::kUsbIcon;
      break;
    case CONTENT_SETTINGS_TYPE_SERIAL_CHOOSER_DATA:
      icon = &vector_icons::kSerialPortIcon;
      break;
    default:
      NOTREACHED();
      break;
  }

  if (deleted) {
    return gfx::CreateVectorIconWithBadge(
        *icon, kVectorIconSize,
        color_utils::DeriveDefaultIconColor(related_text_color),
        kBlockedBadgeIcon);
  }
  return gfx::CreateVectorIcon(
      *icon, kVectorIconSize,
      color_utils::DeriveDefaultIconColor(related_text_color));
}
