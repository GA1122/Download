bool AppListControllerDelegate::HasOptionsPage(
    Profile* profile,
    const std::string& app_id) {
  const extensions::Extension* extension = GetExtension(profile, app_id);
  return extensions::util::IsAppLaunchableWithoutEnabling(app_id, profile) &&
         extension && extensions::OptionsPageInfo::HasOptionsPage(extension);
}
