ChromeBrowserMainParts::ChromeBrowserMainParts(
    const content::MainFunctionParams& parameters)
    : parameters_(parameters),
      parsed_command_line_(parameters.command_line),
      result_code_(content::RESULT_CODE_NORMAL_EXIT),
      startup_watcher_(new StartupTimeBomb()),
      shutdown_watcher_(new ShutdownWatcherHelper()),
      browser_field_trials_(parameters.command_line),
      profile_(NULL),
      run_message_loop_(true),
      notify_result_(ProcessSingleton::PROCESS_NONE),
      local_state_(NULL),
      restart_last_session_(false) {
  if (parameters.ui_task)
    browser_defaults::enable_help_app = false;

  net::URLRequest::SetDefaultCookiePolicyToBlock();
}
