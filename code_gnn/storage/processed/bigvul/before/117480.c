void TestingAutomationProvider::IsEnterpriseDevice(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  policy::BrowserPolicyConnector* connector =
      g_browser_process->browser_policy_connector();
  if (!connector) {
    reply.SendError("Unable to access BrowserPolicyConnector");
    return;
  }
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->SetBoolean("enterprise", connector->IsEnterpriseManaged());
  reply.SendSuccess(return_value.get());
}
