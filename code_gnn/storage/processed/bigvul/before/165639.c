std::wstring GetActiveSetupPath() {
  return std::wstring(
             L"Software\\Microsoft\\Active Setup\\Installed Components\\")
      .append(InstallDetails::Get().mode().active_setup_guid);
}
