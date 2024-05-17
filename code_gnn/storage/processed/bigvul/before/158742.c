void GLES2DecoderImpl::ProcessDescheduleUntilFinished() {
  if (deschedule_until_finished_fences_.size() < 2)
    return;
  DCHECK_EQ(2u, deschedule_until_finished_fences_.size());

  if (!deschedule_until_finished_fences_[0]->HasCompleted())
    return;

  TRACE_EVENT_ASYNC_END0("cc", "GLES2DecoderImpl::DescheduleUntilFinished",
                         this);
  deschedule_until_finished_fences_.erase(
      deschedule_until_finished_fences_.begin());
  client_->OnRescheduleAfterFinished();
}
