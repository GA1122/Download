DeviceTokenFetcher::DeviceTokenFetcher(
    DeviceManagementService* service,
    CloudPolicyCacheBase* cache,
    CloudPolicyDataStore* data_store,
    PolicyNotifier* notifier) {
  Initialize(service,
             cache,
             data_store,
             notifier,
             new DelayedWorkScheduler);
}
