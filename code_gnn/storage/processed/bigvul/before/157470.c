base::string16 GetAppListAppName() {
  static const base::char16 kAppListAppNameSuffix[] = L"AppList";
  base::string16 app_name(install_static::GetBaseAppId());
  app_name.append(kAppListAppNameSuffix);
  return app_name;
}
