bool ReportingIsEnforcedByPolicy(bool* crash_reporting_enabled) {
  std::wstring policies_path = L"SOFTWARE\\Policies\\";
  AppendChromeInstallSubDirectory(InstallDetails::Get().mode(),
                                  false  , &policies_path);
  DWORD value = 0;

  if (nt::QueryRegValueDWORD(nt::HKLM, nt::NONE, policies_path.c_str(),
                             kMetricsReportingEnabled, &value)) {
    *crash_reporting_enabled = (value != 0);
    return true;
  }

  if (nt::QueryRegValueDWORD(nt::HKCU, nt::NONE, policies_path.c_str(),
                             kMetricsReportingEnabled, &value)) {
    *crash_reporting_enabled = (value != 0);
    return true;
  }

  return false;
}
