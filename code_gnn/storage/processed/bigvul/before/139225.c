void RenderProcessHostImpl::ShutdownForBadMessage(
    CrashReportMode crash_report_mode) {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kDisableKillAfterBadIPC))
    return;

  if (run_renderer_in_process()) {
    CHECK(false);
  }

  Shutdown(RESULT_CODE_KILLED_BAD_MESSAGE, false);

  if (crash_report_mode == CrashReportMode::GENERATE_CRASH_DUMP) {
    base::debug::DumpWithoutCrashing();
  }

  BrowserChildProcessHostImpl::HistogramBadMessageTerminated(
      PROCESS_TYPE_RENDERER);
}
