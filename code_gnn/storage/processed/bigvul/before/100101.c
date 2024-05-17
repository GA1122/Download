BrowserActionButton::BrowserActionButton(Extension* extension,
                                         BrowserActionsContainer* panel)
    : MenuButton(this, L"", NULL, false),
      browser_action_(extension->browser_action()),
      extension_(extension),
      tracker_(NULL),
      showing_context_menu_(false),
      panel_(panel) {
  set_alignment(TextButton::ALIGN_CENTER);


  registrar_.Add(this, NotificationType::EXTENSION_BROWSER_ACTION_UPDATED,
                 Source<ExtensionAction>(browser_action_));

  std::string relative_path = browser_action_->default_icon_path();
  if (relative_path.empty())
    return;

  tracker_ = new ImageLoadingTracker(this, 1);
  tracker_->PostLoadImageTask(
      extension->GetResource(relative_path),
      gfx::Size(Extension::kBrowserActionIconMaxSize,
                Extension::kBrowserActionIconMaxSize));
}
