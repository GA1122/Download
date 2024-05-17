CloudPolicyController::CloudPolicyController(
    DeviceManagementService* service,
    CloudPolicyCacheBase* cache,
    DeviceTokenFetcher* token_fetcher,
    CloudPolicyDataStore* data_store,
    PolicyNotifier* notifier) {
  Initialize(service,
             cache,
             token_fetcher,
             data_store,
             notifier,
             new DelayedWorkScheduler);
}
