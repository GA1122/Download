void InitializeProcessType() {
  assert(g_process_type == ProcessType::UNINITIALIZED);
  std::wstring process_type =
      GetSwitchValueFromCommandLine(::GetCommandLine(), kProcessType);
  g_process_type = GetProcessType(process_type);
}
