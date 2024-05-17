void AppCacheHost::PrepareForTransfer() {
  DCHECK(!associated_cache());
  DCHECK(!is_selection_pending());
  DCHECK(!group_being_updated_.get());
  host_id_ = kAppCacheNoHostId;
  frontend_ = NULL;
}
