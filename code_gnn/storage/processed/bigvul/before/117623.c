bool BrowserInit::LaunchWithProfile::CheckIfAutoLaunched(Profile* profile) {
#if defined(OS_WIN)
  if (!auto_launch_trial::IsInAutoLaunchGroup())
    return false;

  int infobar_shown =
      profile->GetPrefs()->GetInteger(prefs::kShownAutoLaunchInfobar);
  if (infobar_shown >= kMaxInfobarShown)
    return false;

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kAutoLaunchAtStartup) ||
      first_run::IsChromeFirstRun()) {
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            base::Bind(&CheckAutoLaunchCallback));
    return true;
  }
#endif
  return false;
}
