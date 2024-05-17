HRESULT CGaiaCredentialBase::GetBaseGlsCommandline(
    base::CommandLine* command_line) {
  DCHECK(command_line);

  base::FilePath gls_path =
      chrome_launcher_support::GetChromePathForInstallationLevel(
          chrome_launcher_support::SYSTEM_LEVEL_INSTALLATION, false);

  constexpr wchar_t kGlsPath[] = L"gls_path";

  wchar_t custom_gls_path_value[MAX_PATH];
  ULONG path_len = base::size(custom_gls_path_value);
  HRESULT hr = GetGlobalFlag(kGlsPath, custom_gls_path_value, &path_len);
  if (SUCCEEDED(hr)) {
    base::FilePath custom_gls_path(custom_gls_path_value);
    if (base::PathExists(custom_gls_path)) {
      gls_path = custom_gls_path;
    } else {
      LOGFN(ERROR) << "Specified gls path ('" << custom_gls_path.value()
                   << "') does not exist, using default gls path.";
    }
  }

  if (gls_path.empty()) {
    LOGFN(ERROR) << "No path to chrome.exe could be found.";
    return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
  }

  command_line->SetProgram(gls_path);

  LOGFN(INFO) << "App exe: " << command_line->GetProgram().value();

  command_line->AppendSwitch(kGcpwSigninSwitch);

  wchar_t endpoint_url_setting[256];
  ULONG endpoint_url_length = base::size(endpoint_url_setting);
  if (SUCCEEDED(GetGlobalFlag(L"ep_url", endpoint_url_setting,
                              &endpoint_url_length)) &&
      endpoint_url_setting[0]) {
    GURL endpoint_url(endpoint_url_setting);
    if (endpoint_url.is_valid()) {
      command_line->AppendSwitchASCII(switches::kGaiaUrl,
                                      endpoint_url.GetWithEmptyPath().spec());
      command_line->AppendSwitchASCII(kGcpwEndpointPathSwitch,
                                      endpoint_url.path().substr(1));
    }
  }

  command_line->AppendSwitchNative("lang", GetSelectedLanguage());

  return S_OK;
}
