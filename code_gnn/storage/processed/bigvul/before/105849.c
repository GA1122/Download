void UrlmonUrlRequest::Stop() {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  DCHECK((status_.get_state() != Status::DONE) == (binding_ != NULL));
  Status::State state = status_.get_state();
  delegate_ = NULL;

  if (terminate_requested())
    return;

  switch (state) {
    case Status::WORKING:
      status_.Cancel();
      if (binding_)
        binding_->Abort();
      break;

    case Status::ABORTING:
      status_.Cancel();
      break;

    case Status::DONE:
      status_.Cancel();
      NotifyDelegateAndDie();
      break;
  }
}
