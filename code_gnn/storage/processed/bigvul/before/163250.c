BrowserMainLoop::BrowserMainLoop(const MainFunctionParams& parameters)
    : parameters_(parameters),
      parsed_command_line_(parameters.command_line),
      result_code_(RESULT_CODE_NORMAL_EXIT),
      created_threads_(false),
      is_tracing_startup_for_duration_(
          parameters.command_line.HasSwitch(switches::kTraceStartup) ||
          (tracing::TraceConfigFile::GetInstance()->IsEnabled() &&
           tracing::TraceConfigFile::GetInstance()->GetStartupDuration() > 0)) {
  DCHECK(!g_current_browser_main_loop);
  g_current_browser_main_loop = this;

  base::TaskScheduler::Create("");
}
