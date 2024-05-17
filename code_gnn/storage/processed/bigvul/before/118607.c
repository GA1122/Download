void AppLauncherHandler::StopShowingAppLauncherPromo(
    const base::ListValue* args) {
#if defined(ENABLE_APP_LIST)
  g_browser_process->local_state()->SetBoolean(
      prefs::kShowAppLauncherPromo, false);
  RecordAppLauncherPromoHistogram(apps::APP_LAUNCHER_PROMO_DISMISSED);
#endif
}
