 bool NaClProcessHost::StartWithLaunchedProcess() {
#if defined(OS_LINUX)
  if (wait_for_nacl_gdb_) {
    if (LaunchNaClGdb(base::GetProcId(process_->GetData().handle))) {
      return true;
    }
    DLOG(ERROR) << "Failed to launch debugger";
  }
#endif

  NaClBrowser* nacl_browser = NaClBrowser::GetInstance();

  if (nacl_browser->IsReady()) {
    return SendStart();
  } else if (nacl_browser->IsOk()) {
    nacl_browser->WaitForResources(
        base::Bind(&NaClProcessHost::OnResourcesReady,
                   weak_factory_.GetWeakPtr()));
    return true;
  } else {
    return false;
  }
}
