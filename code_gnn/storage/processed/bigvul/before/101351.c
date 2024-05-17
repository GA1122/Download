SessionModelAssociator::SessionModelAssociator(ProfileSyncService* sync_service)
    : tab_pool_(sync_service),
      local_session_syncid_(sync_api::kInvalidId),
      sync_service_(sync_service),
      setup_for_test_(false),
      waiting_for_change_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(test_method_factory_(this)) {
  DCHECK(CalledOnValidThread());
  DCHECK(sync_service_);
}
