void DownloadItemImpl::TransitionTo(DownloadState new_state) {
  if (state_ == new_state)
    return;

  DownloadState old_state = state_;
  state_ = new_state;

  switch (state_) {
    case COMPLETE:
      bound_net_log_.AddEvent(
          net::NetLog::TYPE_DOWNLOAD_ITEM_FINISHED,
          base::Bind(&download_net_logs::ItemFinishedCallback,
                     received_bytes_, &hash_));
      break;
    case INTERRUPTED:
      bound_net_log_.AddEvent(
          net::NetLog::TYPE_DOWNLOAD_ITEM_INTERRUPTED,
          base::Bind(&download_net_logs::ItemInterruptedCallback,
                     last_reason_, received_bytes_, &hash_state_));
      break;
    case CANCELLED:
      bound_net_log_.AddEvent(
          net::NetLog::TYPE_DOWNLOAD_ITEM_CANCELED,
          base::Bind(&download_net_logs::ItemCanceledCallback,
                     received_bytes_, &hash_state_));
      break;
    default:
      break;
  }

  VLOG(20) << " " << __FUNCTION__ << "()" << " this = " << DebugString(true);

  UpdateObservers();

  bool is_done = (state_ != IN_PROGRESS);
  bool was_done = (old_state != IN_PROGRESS);
  if (is_done && !was_done)
    bound_net_log_.EndEvent(net::NetLog::TYPE_DOWNLOAD_ITEM_ACTIVE);
}
