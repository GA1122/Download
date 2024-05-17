const wchar_t* GetSandboxSidPrefix() {
  return InstallDetails::Get().mode().sandbox_sid_prefix;
}
