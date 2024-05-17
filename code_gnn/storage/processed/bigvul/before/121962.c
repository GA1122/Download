 void AppListController::DisableAppList() {
  PrefService* local_state = g_browser_process->local_state();
  local_state->SetBoolean(apps::prefs::kAppLauncherHasBeenEnabled, false);
}
