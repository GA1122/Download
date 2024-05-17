base::FilePath PlatformCrashpadInitialization(
    bool initial_client,
    bool browser_process,
    bool embedded_handler,
    const std::string& user_data_dir,
    const base::FilePath& exe_path,
    const std::vector<std::string>& initial_arguments) {
  DCHECK_EQ(initial_client, browser_process);
  DCHECK(initial_arguments.empty());

  DCHECK(!embedded_handler);
  DCHECK(exe_path.empty());

  g_is_browser = browser_process;

  bool dump_at_crash = true;
#if defined(OS_ANDROID)
  base::android::SetJavaExceptionCallback(SetJavaExceptionInfo);

  unsigned int dump_percentage =
      GetCrashReporterClient()->GetCrashDumpPercentageForWebView();
  if (dump_percentage < 100 && rand() % 100 >= dump_percentage) {
    dump_at_crash = false;
  }
#endif   

  if (browser_process) {
    HandlerStarter* starter = HandlerStarter::Get();
    return starter->Initialize(dump_at_crash);
  }

  crashpad::SandboxedHandler* handler = crashpad::SandboxedHandler::Get();
  bool result = handler->Initialize(dump_at_crash);
  DCHECK(result);

  return base::FilePath();
}
