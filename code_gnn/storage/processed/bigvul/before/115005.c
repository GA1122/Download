void TestingAutomationProvider::RefreshPolicies(
    base::DictionaryValue* args,
    IPC::Message* reply_message) {
#if !defined(ENABLE_CONFIGURATION_POLICY)
  AutomationJSONReply(this, reply_message).SendError(
      "Configuration Policy disabled");
#else
  policy::BrowserPolicyConnector* connector =
      g_browser_process->browser_policy_connector();
  new PolicyUpdatesObserver(this, reply_message, connector);
  connector->RefreshPolicies();
#endif
}
