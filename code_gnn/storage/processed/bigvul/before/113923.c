InvalidationNotifier::InvalidationNotifier(
    scoped_ptr<notifier::PushClient> push_client,
    const InvalidationVersionMap& initial_max_invalidation_versions,
    const std::string& initial_invalidation_state,
    const WeakHandle<InvalidationStateTracker>& invalidation_state_tracker,
    const std::string& client_info)
    : state_(STOPPED),
      initial_max_invalidation_versions_(initial_max_invalidation_versions),
      invalidation_state_tracker_(invalidation_state_tracker),
      client_info_(client_info),
      invalidation_state_(initial_invalidation_state),
      invalidation_client_(push_client.Pass()) {
}
