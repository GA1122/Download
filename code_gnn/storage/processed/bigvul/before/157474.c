DefaultWebClientSetPermission GetDefaultWebClientSetPermission() {
  if (!install_static::SupportsSetAsDefaultBrowser())
    return SET_DEFAULT_NOT_ALLOWED;
  if (ShellUtil::CanMakeChromeDefaultUnattended())
    return SET_DEFAULT_UNATTENDED;
  return SET_DEFAULT_INTERACTIVE;
}
