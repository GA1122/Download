unsigned __stdcall CGaiaCredentialBase::WaitForLoginUI(void* param) {
  USES_CONVERSION;
  DCHECK(param);
  std::unique_ptr<UIProcessInfo> uiprocinfo(
      reinterpret_cast<UIProcessInfo*>(param));

  base::win::ScopedCOMInitializer com_initializer(
      base::win::ScopedCOMInitializer::kMTA);
  if (!com_initializer.Succeeded()) {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    LOGFN(ERROR) << "ScopedCOMInitializer failed hr=" << putHR(hr);
    return hr;
  }

  CComBSTR status_text;
  DWORD exit_code;
  std::string json_result;
  HRESULT hr = WaitForLoginUIAndGetResult(uiprocinfo.get(), &json_result,
                                          &exit_code, &status_text);
  if (SUCCEEDED(hr)) {
    hr = uiprocinfo->credential->OnUserAuthenticated(
        CComBSTR(A2COLE(json_result.c_str())), &status_text);
  }

  if (FAILED(hr) && exit_code != kUiecKilled) {
    if (hr != E_ABORT)
      LOGFN(ERROR) << "WaitForLoginUIAndGetResult hr=" << putHR(hr);

    LONG sts = hr == E_ABORT ? STATUS_SUCCESS : HRESULT_CODE(hr);

    DCHECK(sts == STATUS_SUCCESS || status_text != nullptr);
    hr = uiprocinfo->credential->ReportError(sts, STATUS_SUCCESS, status_text);
    if (FAILED(hr))
      LOGFN(ERROR) << "uiprocinfo->credential->ReportError hr=" << putHR(hr);
  }

  LOGFN(INFO) << "done";
  return 0;
}
