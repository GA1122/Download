SyncBackendHost::Core::Core(SyncBackendHost* backend)
    : host_(backend),
      syncapi_(new sync_api::SyncManager()),
      sync_manager_observer_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      parent_router_(NULL),
      processing_passphrase_(false),
      deferred_nudge_for_cleanup_requested_(false) {
}
