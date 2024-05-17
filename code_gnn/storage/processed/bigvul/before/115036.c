void TestingAutomationProvider::SetPolicies(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  scoped_ptr<AutomationJSONReply> reply(
      new AutomationJSONReply(this, reply_message));

#if !defined(ENABLE_CONFIGURATION_POLICY) || defined(OFFICIAL_BUILD)
  reply->SendError("Configuration Policy disabled");
#else
  const policy::PolicyDefinitionList* list =
      policy::GetChromePolicyDefinitionList();
  policy::BrowserPolicyConnector* connector =
      g_browser_process->browser_policy_connector();
  struct {
    std::string name;
    policy::ConfigurationPolicyProvider* provider;
  } providers[] = {
    { "managed_cloud",        connector->GetManagedCloudProvider()        },
    { "managed_platform",     connector->GetManagedPlatformProvider()     },
    { "recommended_cloud",    connector->GetRecommendedCloudProvider()    },
    { "recommended_platform", connector->GetRecommendedPlatformProvider() }
  };
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(providers); ++i) {
    DictionaryValue* policies = NULL;
    if (args->GetDictionary(providers[i].name, &policies) &&
        policies &&
        !providers[i].provider) {
      reply->SendError("Provider not available: " + providers[i].name);
      return;
    }
  }
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(providers); ++i) {
    DictionaryValue* policies = NULL;
    if (args->GetDictionary(providers[i].name, &policies) && policies) {
      policy::PolicyMap* map = new policy::PolicyMap;
      map->LoadFrom(policies, list);
      providers[i].provider->OverridePolicies(map);
    }
  }

  PolicyUpdatesObserver::PostCallbackAfterPolicyUpdates(
      base::Bind(&AutomationJSONReply::SendSuccess,
                 base::Owned(reply.release()),
                 static_cast<const Value*>(NULL)));
#endif   
}
