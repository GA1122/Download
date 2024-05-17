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
    auto* policy = ChildProcessSecurityPolicyImpl::GetInstance();
    base::debug::SetCrashKeyString(bad_message::GetKilledProcessOriginLockKey(),
                                   policy->GetOriginLock(GetID()).spec());

    std::string site_isolation_mode;
    if (SiteIsolationPolicy::UseDedicatedProcessesForAllSites())
      site_isolation_mode += "spp ";
    if (SiteIsolationPolicy::IsTopDocumentIsolationEnabled())
      site_isolation_mode += "tdi ";
    if (SiteIsolationPolicy::AreIsolatedOriginsEnabled())
      site_isolation_mode += "io ";

    static auto* isolation_mode_key = base::debug::AllocateCrashKeyString(
        "site_isolation_mode", base::debug::CrashKeySize::Size32);
    base::debug::SetCrashKeyString(isolation_mode_key, site_isolation_mode);

    base::debug::DumpWithoutCrashing();
  }

  BrowserChildProcessHostImpl::HistogramBadMessageTerminated(
      PROCESS_TYPE_RENDERER);
}
