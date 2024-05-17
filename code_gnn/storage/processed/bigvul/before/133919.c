void AppListControllerDelegate::GetApps(Profile* profile,
                                        extensions::ExtensionSet* out_apps) {
  ExtensionRegistry* registry = ExtensionRegistry::Get(profile);
  DCHECK(registry);
  out_apps->InsertAll(registry->enabled_extensions());
  out_apps->InsertAll(registry->disabled_extensions());
  out_apps->InsertAll(registry->terminated_extensions());
}
