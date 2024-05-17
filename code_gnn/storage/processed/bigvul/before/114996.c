void TestingAutomationProvider::OnRemoveProvider() {
  if (g_browser_process)
    g_browser_process->GetAutomationProviderList()->RemoveProvider(this);
}
