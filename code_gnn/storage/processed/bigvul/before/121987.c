void MigrateAppLauncherEnabledPref() {
  PrefService* prefs = g_browser_process->local_state();
  if (prefs->HasPrefPath(apps::prefs::kAppLauncherIsEnabled)) {
    prefs->SetBoolean(apps::prefs::kAppLauncherHasBeenEnabled,
                      prefs->GetBoolean(apps::prefs::kAppLauncherIsEnabled));
    prefs->ClearPref(apps::prefs::kAppLauncherIsEnabled);
  }
}
