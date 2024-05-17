void Browser::CreateBrowserWindow() {
  DCHECK(!window_);

  window_ = BrowserWindow::CreateBrowserWindow(this);

#if defined(OS_WIN)
  {
    base::ThreadRestrictions::ScopedAllowIO allow_io;

    app::win::SetAppIdForWindow(
        type_ & TYPE_APP ?
        ShellIntegration::GetAppId(UTF8ToWide(app_name_), profile_->GetPath()) :
        ShellIntegration::GetChromiumAppId(profile_->GetPath()),
        window()->GetNativeHandle());
  }
#endif

  NotificationService::current()->Notify(
      NotificationType::BROWSER_WINDOW_READY,
      Source<Browser>(this),
      NotificationService::NoDetails());

  PrefService* local_state = g_browser_process->local_state();
  if (!local_state)
    return;
  if (local_state->FindPreference(prefs::kShouldShowFirstRunBubble) &&
      local_state->GetBoolean(prefs::kShouldShowFirstRunBubble)) {
    FirstRun::BubbleType bubble_type = FirstRun::LARGE_BUBBLE;
    if (local_state->
        FindPreference(prefs::kShouldUseOEMFirstRunBubble) &&
        local_state->GetBoolean(prefs::kShouldUseOEMFirstRunBubble)) {
      bubble_type = FirstRun::OEM_BUBBLE;
    } else if (local_state->
        FindPreference(prefs::kShouldUseMinimalFirstRunBubble) &&
        local_state->GetBoolean(prefs::kShouldUseMinimalFirstRunBubble)) {
      bubble_type = FirstRun::MINIMAL_BUBBLE;
    }
    local_state->ClearPref(prefs::kShouldShowFirstRunBubble);
    window_->GetLocationBar()->ShowFirstRunBubble(bubble_type);
  }
  if (local_state->FindPreference(
      prefs::kAutoFillPersonalDataManagerFirstRun) &&
      local_state->GetBoolean(prefs::kAutoFillPersonalDataManagerFirstRun)) {
#if defined(OS_WIN)
    ImportAutofillDataWin(profile_->GetPersonalDataManager());
#endif   
    local_state->ClearPref(prefs::kAutoFillPersonalDataManagerFirstRun);
  }
}
