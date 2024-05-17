HRESULT CGaiaCredentialBase::CreateAndRunLogonStub() {
  LOGFN(INFO);

  base::CommandLine command_line(base::CommandLine::NO_PROGRAM);
  HRESULT hr = GetGlsCommandline(&command_line);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "GetGlsCommandline hr=" << putHR(hr);
    return hr;
  }

  std::unique_ptr<UIProcessInfo> uiprocinfo(new UIProcessInfo);
  PSID logon_sid;
  hr = CreateGaiaLogonToken(&uiprocinfo->logon_token, &logon_sid);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "CreateGaiaLogonToken hr=" << putHR(hr);
    return hr;
  }

  OSProcessManager* process_manager = OSProcessManager::Get();
  hr = process_manager->SetupPermissionsForLogonSid(logon_sid);
  LocalFree(logon_sid);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "SetupPermissionsForLogonSid hr=" << putHR(hr);
    return hr;
  }

  hr = ForkGaiaLogonStub(process_manager, command_line, uiprocinfo.get());
  if (FAILED(hr)) {
    LOGFN(ERROR) << "ForkGaiaLogonStub hr=" << putHR(hr);
    return hr;
  }

  DCHECK_EQ(logon_ui_process_, INVALID_HANDLE_VALUE);
  logon_ui_process_ = uiprocinfo->procinfo.process_handle();

  uiprocinfo->credential = this;

  unsigned int wait_thread_id;
  UIProcessInfo* puiprocinfo = uiprocinfo.release();
  uintptr_t wait_thread = _beginthreadex(nullptr, 0, WaitForLoginUI,
                                         puiprocinfo, 0, &wait_thread_id);
  if (wait_thread != 0) {
    LOGFN(INFO) << "Started wait thread id=" << wait_thread_id;
    ::CloseHandle(reinterpret_cast<HANDLE>(wait_thread));
  } else {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    LOGFN(ERROR) << "Unable to start wait thread hr=" << putHR(hr);
    ::TerminateProcess(puiprocinfo->procinfo.process_handle(), kUiecKilled);
    delete puiprocinfo;
    return hr;
  }

  LOGFN(INFO) << "cleaning up";
  return S_OK;
}
