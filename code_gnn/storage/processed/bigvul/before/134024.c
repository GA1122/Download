void ExtensionAppItem::UpdateIcon() {
  gfx::ImageSkia icon = installing_icon_;

  if (GetExtension() && icon_) {
    icon = icon_->image_skia();
    const bool enabled = extensions::util::IsAppLaunchable(extension_id_,
                                                           profile_);
    if (!enabled)
      icon = CreateDisabledIcon(icon);

    if (GetExtension()->from_bookmark())
      icon = gfx::ImageSkia(new RoundedCornersImageSource(icon), icon.size());
  }
  has_overlay_ = NeedsOverlay();
  if (has_overlay_)
    icon = gfx::ImageSkia(new ShortcutOverlayImageSource(icon), icon.size());

  SetIcon(icon);
}
