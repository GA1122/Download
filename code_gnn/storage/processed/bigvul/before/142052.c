HRESULT CGaiaCredentialBase::ForkGaiaLogonStub(
    OSProcessManager* process_manager,
    const base::CommandLine& command_line,
    UIProcessInfo* uiprocinfo) {
  LOGFN(INFO);
  DCHECK(process_manager);
  DCHECK(uiprocinfo);

  ScopedStartupInfo startupinfo(kDesktopFullName);

  HRESULT hr =
      InitializeStdHandles(CommDirection::kChildToParentOnly, kStdOutput,
                           &startupinfo, &uiprocinfo->parent_handles);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "InitializeStdHandles hr=" << putHR(hr);
    return hr;
  }

  hr = process_manager->CreateProcessWithToken(
      uiprocinfo->logon_token, command_line, startupinfo.GetInfo(),
      &uiprocinfo->procinfo);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "process_manager->CreateProcessWithToken hr=" << putHR(hr);
    return hr;
  }

  LOGFN(INFO) << "pid=" << uiprocinfo->procinfo.process_id()
              << " tid=" << uiprocinfo->procinfo.thread_id();


  if (::ResumeThread(uiprocinfo->procinfo.thread_handle()) ==
      static_cast<DWORD>(-1)) {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    LOGFN(ERROR) << "ResumeThread hr=" << putHR(hr);
    ::TerminateProcess(uiprocinfo->procinfo.process_handle(), kUiecKilled);
    return hr;
  }

  DWORD ret = ::WaitForInputIdle(uiprocinfo->procinfo.process_handle(), 10000);
  if (ret != 0)
    LOGFN(INFO) << "WaitForInputIdle, ret=" << ret;

  return S_OK;
}
