ExtensionInstalledBubble::ExtensionInstalledBubble(const Extension* extension,
                                                   Browser *browser,
                                                   const SkBitmap& icon)
    : extension_(extension),
      browser_(browser),
      icon_(icon),
      animation_wait_retries_(0) {
  if (extension->is_app()) {
    type_ = APP;
  } else if (!extension_->omnibox_keyword().empty()) {
    type_ = OMNIBOX_KEYWORD;
  } else if (extension_->browser_action()) {
    type_ = BROWSER_ACTION;
  } else if (extension->page_action() &&
             !extension->page_action()->default_icon_path().empty()) {
    type_ = PAGE_ACTION;
  } else {
    type_ = GENERIC;
  }

  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_LOADED,
      content::Source<Profile>(browser->profile()));
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNLOADED,
      content::Source<Profile>(browser->profile()));
}
