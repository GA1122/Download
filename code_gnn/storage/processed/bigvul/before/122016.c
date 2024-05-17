bool IsAppLauncherEnabledAtLevel(InstallationLevel level) {
  string16 uninstall_arguments;
  if (GetClientStateValue(level,
                          kAppHostAppId,
                          kUninstallArgumentsField,
                          &uninstall_arguments)) {
    return CommandLine::FromString(L"dummy.exe " + uninstall_arguments)
        .HasSwitch(kChromeAppLauncher) &&
        !GetAppHostPathForInstallationLevel(level).empty();
  }
  return false;
}
