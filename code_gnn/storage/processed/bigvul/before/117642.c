bool BrowserInit::LaunchWithProfile::Launch(
    Profile* profile,
    const std::vector<GURL>& urls_to_open,
    bool process_startup) {
  DCHECK(profile);
  profile_ = profile;

  if (command_line_.HasSwitch(switches::kDnsLogDetails))
    chrome_browser_net::EnablePredictorDetailedLog(true);
  if (command_line_.HasSwitch(switches::kDnsPrefetchDisable) &&
      profile->GetNetworkPredictor()) {
    profile->GetNetworkPredictor()->EnablePredictor(false);
  }

  if (command_line_.HasSwitch(switches::kDumpHistogramsOnExit))
    base::StatisticsRecorder::set_dump_on_exit(true);

  if (command_line_.HasSwitch(switches::kRemoteDebuggingPort)) {
    std::string port_str =
        command_line_.GetSwitchValueASCII(switches::kRemoteDebuggingPort);
    int64 port;
    if (base::StringToInt64(port_str, &port) && port > 0 && port < 65535) {
      std::string frontend_str;
      if (command_line_.HasSwitch(switches::kRemoteDebuggingFrontend)) {
        frontend_str = command_line_.GetSwitchValueASCII(
            switches::kRemoteDebuggingFrontend);
      }
      g_browser_process->InitDevToolsHttpProtocolHandler(
          profile,
          "127.0.0.1",
          static_cast<int>(port),
          frontend_str);
    } else {
      DLOG(WARNING) << "Invalid http debugger port number " << port;
    }
  }

  if (OpenApplicationWindow(profile)) {
    RecordLaunchModeHistogram(LM_AS_WEBAPP);
  } else {
    RecordLaunchModeHistogram(urls_to_open.empty()?
                              LM_TO_BE_DECIDED : LM_WITH_URLS);
    ProcessLaunchURLs(process_startup, urls_to_open);

    OpenApplicationTab(profile);

    if (process_startup) {
      if (browser_defaults::kOSSupportsOtherBrowsers &&
          !command_line_.HasSwitch(switches::kNoDefaultBrowserCheck)) {
        if (!CheckIfAutoLaunched(profile)) {
          CheckDefaultBrowser(profile);
        }
      }
#if defined(OS_MACOSX)
      KeystoneInfoBar::PromotionInfoBar(profile);
#endif
    }
  }

#if defined(OS_WIN)
  if (command_line_.HasSwitch(switches::kPrint)) {
    Browser* browser = BrowserList::GetLastActive();
    browser->Print();
  }
#endif

  if (!command_line_.HasSwitch(switches::kNoEvents)) {
    FilePath script_path;
    PathService::Get(chrome::FILE_RECORDED_SCRIPT, &script_path);

    bool record_mode = command_line_.HasSwitch(switches::kRecordMode);
    bool playback_mode = command_line_.HasSwitch(switches::kPlaybackMode);

    if (record_mode && chrome::kRecordModeEnabled)
      base::EventRecorder::current()->StartRecording(script_path);
    if (playback_mode)
      base::EventRecorder::current()->StartPlayback(script_path);
  }

#if defined(OS_WIN)
  if (process_startup)
    ShellIntegration::MigrateChromiumShortcuts();
#endif   

  return true;
}
