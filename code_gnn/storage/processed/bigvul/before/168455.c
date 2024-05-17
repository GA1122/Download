bool ExternalProtocolDialog::Accept() {
  UMA_HISTOGRAM_LONG_TIMES("clickjacking.launch_url",
                           base::TimeTicks::Now() - creation_time_);

  const bool remember = remember_decision_checkbox_->checked();
  ExternalProtocolHandler::RecordHandleStateMetrics(
      remember, ExternalProtocolHandler::DONT_BLOCK);

  delegate_->DoAccept(delegate_->url(), remember);

  return true;
}
