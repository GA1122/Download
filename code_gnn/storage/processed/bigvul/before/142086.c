void CGaiaCredentialBase::TerminateLogonProcess() {
  if (logon_ui_process_ != INVALID_HANDLE_VALUE) {
    LOGFN(INFO) << "Attempting to kill logon UI process";
    ::TerminateProcess(logon_ui_process_, kUiecKilled);
    logon_ui_process_ = INVALID_HANDLE_VALUE;
  }
}
