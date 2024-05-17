DeviceTokenFetcher::DeviceTokenFetcher(
    DeviceManagementService* service,
    CloudPolicyCacheBase* cache,
    CloudPolicyDataStore* data_store,
    PolicyNotifier* notifier,
    DelayedWorkScheduler* scheduler) {
  Initialize(service, cache, data_store, notifier, scheduler);
}
