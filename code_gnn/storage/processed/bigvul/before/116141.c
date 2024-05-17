void ResourceDispatcherHostImpl::MaybeUpdateUploadProgress(
    ResourceRequestInfoImpl *info,
    net::URLRequest *request) {

  if (!info->GetUploadSize() || info->waiting_for_upload_progress_ack())
    return;

  uint64 size = info->GetUploadSize();
  uint64 position = request->GetUploadProgress();
  if (position == info->last_upload_position())
    return;   

  const uint64 kHalfPercentIncrements = 200;
  const TimeDelta kOneSecond = TimeDelta::FromMilliseconds(1000);

  uint64 amt_since_last = position - info->last_upload_position();
  TimeDelta time_since_last = TimeTicks::Now() - info->last_upload_ticks();

  bool is_finished = (size == position);
  bool enough_new_progress = (amt_since_last > (size / kHalfPercentIncrements));
  bool too_much_time_passed = time_since_last > kOneSecond;

  if (is_finished || enough_new_progress || too_much_time_passed) {
    if (request->load_flags() & net::LOAD_ENABLE_UPLOAD_PROGRESS) {
      info->resource_handler()->OnUploadProgress(info->GetRequestID(),
                                                 position, size);
      info->set_waiting_for_upload_progress_ack(true);
    }
    info->set_last_upload_ticks(TimeTicks::Now());
    info->set_last_upload_position(position);
  }
}
