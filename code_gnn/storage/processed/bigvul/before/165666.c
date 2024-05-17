const wchar_t* GetProgIdPrefix() {
  return InstallDetails::Get().mode().prog_id_prefix;
}
