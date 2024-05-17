int BackendImpl::SyncInit() {
#if defined(NET_BUILD_STRESS_CACHE)
  up_ticks_ = kTrimDelay * 2;
#endif
  DCHECK(!init_);
  if (init_)
    return net::ERR_FAILED;

  bool create_files = false;
  if (!InitBackingStore(&create_files)) {
    ReportError(ERR_STORAGE_ERROR);
    return net::ERR_FAILED;
  }

  num_refs_ = num_pending_io_ = max_refs_ = 0;
  entry_count_ = byte_count_ = 0;

  bool should_create_timer = false;
  if (!restarted_) {
    buffer_bytes_ = 0;
    trace_object_ = TraceObject::GetTraceObject();
    should_create_timer = true;
  }

  init_ = true;
  Trace("Init");

  if (data_->header.experiment != NO_EXPERIMENT &&
      cache_type_ != net::DISK_CACHE) {
    return net::ERR_FAILED;
  }

  if (!(user_flags_ & kNoRandom)) {
    new_eviction_ = (cache_type_ == net::DISK_CACHE);
  }

  if (!CheckIndex()) {
    ReportError(ERR_INIT_FAILED);
    return net::ERR_FAILED;
  }

  if (!restarted_ && (create_files || !data_->header.num_entries))
    ReportError(ERR_CACHE_CREATED);

  if (!(user_flags_ & kNoRandom) && cache_type_ == net::DISK_CACHE &&
      !InitExperiment(&data_->header, create_files)) {
    return net::ERR_FAILED;
  }

  data_->header.this_id++;
  if (!data_->header.this_id)
    data_->header.this_id++;

  bool previous_crash = (data_->header.crash != 0);
  data_->header.crash = 1;

  if (!block_files_.Init(create_files))
    return net::ERR_FAILED;

  if (cache_type() == net::APP_CACHE) {
    DCHECK(!new_eviction_);
    read_only_ = true;
  } else if (cache_type() == net::SHADER_CACHE) {
    DCHECK(!new_eviction_);
  }

  eviction_.Init(this);

  disabled_ = false;
  if (!InitStats())
    return net::ERR_FAILED;

  disabled_ = !rankings_.Init(this, new_eviction_);

#if defined(STRESS_CACHE_EXTENDED_VALIDATION)
  trace_object_->EnableTracing(false);
  int sc = SelfCheck();
  if (sc < 0 && sc != ERR_NUM_ENTRIES_MISMATCH)
    NOTREACHED();
  trace_object_->EnableTracing(true);
#endif

  if (previous_crash) {
    ReportError(ERR_PREVIOUS_CRASH);
  } else if (!restarted_) {
    ReportError(ERR_NO_ERROR);
  }

  FlushIndex();

  if (!disabled_ && should_create_timer) {
    DCHECK(background_queue_.BackgroundIsCurrentSequence());
    int timer_delay = unit_test_ ? 1000 : 30000;
    timer_.reset(new base::RepeatingTimer());
    timer_->Start(FROM_HERE, TimeDelta::FromMilliseconds(timer_delay), this,
                  &BackendImpl::OnStatsTimer);
  }

  return disabled_ ? net::ERR_FAILED : net::OK;
}
