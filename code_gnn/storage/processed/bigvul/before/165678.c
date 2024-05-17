bool IsNonBrowserProcess() {
  assert(g_process_type != ProcessType::UNINITIALIZED);
  return g_process_type != ProcessType::BROWSER_PROCESS;
}
