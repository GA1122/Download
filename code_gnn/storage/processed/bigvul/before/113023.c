DownloadItemImpl::DownloadItemImpl(Delegate* delegate,
                                   DownloadId download_id,
                                   const DownloadPersistentStoreInfo& info,
                                   const net::BoundNetLog& bound_net_log)
    : download_id_(download_id),
      current_path_(info.path),
      target_path_(info.path),
      target_disposition_(TARGET_DISPOSITION_OVERWRITE),
      url_chain_(1, info.url),
      referrer_url_(info.referrer_url),
      transition_type_(content::PAGE_TRANSITION_LINK),
      has_user_gesture_(false),
      total_bytes_(info.total_bytes),
      received_bytes_(info.received_bytes),
      bytes_per_sec_(0),
      last_reason_(content::DOWNLOAD_INTERRUPT_REASON_NONE),
      start_tick_(base::TimeTicks()),
      state_(static_cast<DownloadState>(info.state)),
      danger_type_(content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS),
      start_time_(info.start_time),
      end_time_(info.end_time),
      db_handle_(info.db_handle),
      delegate_(delegate),
      is_paused_(false),
      open_when_complete_(false),
      file_externally_removed_(false),
      safety_state_(SAFE),
      auto_opened_(false),
      is_persisted_(true),
      is_otr_(false),
      is_temporary_(false),
      all_data_saved_(false),
      opened_(info.opened),
      open_enabled_(true),
      delegate_delayed_complete_(false),
      bound_net_log_(bound_net_log),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_ptr_factory_(this)) {
  delegate_->Attach();
  if (IsInProgress())
    state_ = CANCELLED;
  if (IsComplete())
    all_data_saved_ = true;
  Init(false  ,
       download_net_logs::SRC_HISTORY_IMPORT);
}
