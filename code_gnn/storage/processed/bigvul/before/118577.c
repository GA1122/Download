bool ShouldCreateShortcutFor(Profile* profile, const Extension* extension) {
  return extension->is_platform_app() &&
      extension->location() != extensions::Manifest::COMPONENT &&
      extensions::ui_util::ShouldDisplayInAppLauncher(extension, profile);
}
