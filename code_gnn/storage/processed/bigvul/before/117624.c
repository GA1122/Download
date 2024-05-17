bool BrowserInit::CreateAutomationProvider(const std::string& channel_id,
                                           Profile* profile,
                                           size_t expected_tabs) {
  scoped_refptr<AutomationProviderClass> automation =
      new AutomationProviderClass(profile);

  if (!automation->InitializeChannel(channel_id))
    return false;
  automation->SetExpectedTabCount(expected_tabs);

  AutomationProviderList* list = g_browser_process->GetAutomationProviderList();
  DCHECK(list);
  list->AddProvider(automation);

  return true;
}
