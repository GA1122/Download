bool SessionModelAssociator::DisassociateModels(SyncError* error) {
  DCHECK(CalledOnValidThread());
  synced_session_tracker_.clear();
  tab_map_.clear();
  tab_pool_.clear();
  local_session_syncid_ = sync_api::kInvalidId;
  current_machine_tag_ = "";
  current_session_name_ = "";

  NotificationService::current()->Notify(
      chrome::NOTIFICATION_FOREIGN_SESSION_DISABLED,
      NotificationService::AllSources(),
      NotificationService::NoDetails());
  return true;
}
