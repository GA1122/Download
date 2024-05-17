void NaClProcessHost::EarlyStartup() {
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  NaClBrowser::GetInstance()->EnsureIrtAvailable();
#endif
}
