void SetAsDefaultBrowserUsingSystemSettings(
    const base::Closure& on_finished_callback) {
  base::FilePath chrome_exe;
  if (!base::PathService::Get(base::FILE_EXE, &chrome_exe)) {
    NOTREACHED() << "Error getting app exe path";
    on_finished_callback.Run();
    return;
  }

  std::unique_ptr<DefaultBrowserActionRecorder> recorder(
      new DefaultBrowserActionRecorder(base::Bind(
          base::IgnoreResult(&ShellUtil::ShowMakeChromeDefaultSystemUI),
          chrome_exe)));

  static const wchar_t* const kProtocols[] = {L"http", L"https", nullptr};
  OpenSystemSettingsHelper::Begin(
      kProtocols, base::Bind(&OnSettingsAppFinished, base::Passed(&recorder),
                             on_finished_callback));
}
