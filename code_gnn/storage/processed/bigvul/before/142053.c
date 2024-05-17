HRESULT CGaiaCredentialBase::ForkSaveAccountInfoStub(
    const std::unique_ptr<base::DictionaryValue>& dict,
    BSTR* status_text) {
  LOGFN(INFO);
  DCHECK(status_text);

  ScopedStartupInfo startupinfo;
  StdParentHandles parent_handles;
  HRESULT hr =
      InitializeStdHandles(CommDirection::kParentToChildOnly, kAllStdHandles,
                           &startupinfo, &parent_handles);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "InitializeStdHandles hr=" << putHR(hr);
    *status_text = AllocErrorString(IDS_INTERNAL_ERROR_BASE);
    return hr;
  }

  base::CommandLine command_line(base::CommandLine::NO_PROGRAM);
  hr = GetCommandLineForEntrypoint(CURRENT_MODULE(), L"SaveAccountInfo",
                                   &command_line);
  if (hr == S_FALSE) {
    LOGFN(INFO) << "Not running SAIS";
    return S_OK;
  } else if (FAILED(hr)) {
    LOGFN(ERROR) << "GetCommandLineForEntryPoint hr=" << putHR(hr);
    *status_text = AllocErrorString(IDS_INTERNAL_ERROR_BASE);
    return hr;
  }

  command_line.AppendSwitchASCII(switches::kProcessType,
                                 "gcpw-save-account-info");

  base::win::ScopedProcessInformation procinfo;
  hr = OSProcessManager::Get()->CreateRunningProcess(
      command_line, startupinfo.GetInfo(), &procinfo);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "OSProcessManager::CreateRunningProcess hr=" << putHR(hr);
    *status_text = AllocErrorString(IDS_INTERNAL_ERROR_BASE);
    return hr;
  }

  std::string json;
  if (base::JSONWriter::Write(*dict, &json)) {
    DWORD written;
    if (!::WriteFile(parent_handles.hstdin_write.Get(), json.c_str(),
                     json.length() + 1, &written,  nullptr)) {
      HRESULT hrWrite = HRESULT_FROM_WIN32(::GetLastError());
      LOGFN(ERROR) << "WriteFile hr=" << putHR(hrWrite);
    }
  } else {
    LOGFN(ERROR) << "base::JSONWriter::Write failed";
  }

  return S_OK;
}
