void AppLauncherHandler::OnLocalStatePreferenceChanged() {
#if defined(ENABLE_APP_LIST)
  web_ui()->CallJavascriptFunction(
      "ntp.appLauncherPromoPrefChangeCallback",
      base::FundamentalValue(g_browser_process->local_state()->GetBoolean(
          prefs::kShowAppLauncherPromo)));
#endif
}
