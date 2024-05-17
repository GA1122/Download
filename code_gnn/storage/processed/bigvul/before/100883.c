ExtensionInstallUI::ExtensionInstallUI(Profile* profile)
    : profile_(profile),
      ui_loop_(MessageLoop::current()),
      previous_using_native_theme_(false),
      extension_(NULL),
      delegate_(NULL),
      prompt_type_(NUM_PROMPT_TYPES),
      ALLOW_THIS_IN_INITIALIZER_LIST(tracker_(this)),
      use_app_installed_bubble_(false),
      skip_post_install_ui_(false) {
  if (profile_) {
    const Extension* previous_theme =
        ThemeServiceFactory::GetThemeForProfile(profile_);
    if (previous_theme)
      previous_theme_id_ = previous_theme->id();
    previous_using_native_theme_ =
        ThemeServiceFactory::GetForProfile(profile_)->UsingNativeTheme();
  }
}
