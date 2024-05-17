void ExtensionInstallUI::OnInstallSuccess(const Extension* extension,
                                          SkBitmap* icon) {
  if (skip_post_install_ui_)
    return;

  extension_ = extension;
  SetIcon(icon);

  if (extension->is_theme()) {
    ShowThemeInfoBar(previous_theme_id_, previous_using_native_theme_,
                     extension, profile_);
    return;
  }

  Profile* profile = profile_->GetOriginalProfile();
  Browser* browser = Browser::GetOrCreateTabbedBrowser(profile);
  if (browser->tab_count() == 0)
    browser->AddBlankTab(true);
  browser->window()->Show();

  bool use_bubble_for_apps = false;

#if defined(TOOLKIT_VIEWS)
  CommandLine* cmdline = CommandLine::ForCurrentProcess();
  use_bubble_for_apps = (use_app_installed_bubble_ ||
                         cmdline->HasSwitch(switches::kAppsNewInstallBubble));
#endif

  if (extension->is_app() && !use_bubble_for_apps) {
    ExtensionInstallUI::OpenAppInstalledNTP(browser, extension->id());
    return;
  }

  browser::ShowExtensionInstalledBubble(extension, browser, icon_, profile);
}
