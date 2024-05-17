bool GpuProcessHost::Send(IPC::Message* msg) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (process_->GetHost()->IsChannelOpening()) {
    queued_messages_.push(msg);
    return true;
  }

  bool result = process_->Send(msg);
  if (!result) {
    SendOutstandingReplies(EstablishChannelStatus::GPU_HOST_INVALID);
  }
  return result;
}
