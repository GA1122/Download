void ExtensionInfoBar::SetupIconAndMenu() {
  menu_ = new views::MenuButton(NULL, std::wstring(), this, false);
  menu_->SetVisible(false);
  AddChildView(menu_);

  const Extension* extension = delegate_->extension_host()->extension();
  ExtensionResource icon_resource = extension->GetIconResource(
      Extension::EXTENSION_ICON_BITTY, ExtensionIconSet::MATCH_EXACTLY);
  if (!icon_resource.relative_path().empty()) {
    tracker_.LoadImage(extension, icon_resource,
                       gfx::Size(Extension::EXTENSION_ICON_BITTY,
                                 Extension::EXTENSION_ICON_BITTY),
                       ImageLoadingTracker::DONT_CACHE);
  } else {
    OnImageLoaded(NULL, icon_resource, 0);   
  }
}
