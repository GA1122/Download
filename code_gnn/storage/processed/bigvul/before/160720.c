bool RenderFrameImpl::ScheduleFileChooser(
    const FileChooserParams& params,
    blink::WebFileChooserCompletion* completion) {
  static const size_t kMaximumPendingFileChooseRequests = 4;

  if (IsHidden())
    return false;

  if (file_chooser_completions_.size() > kMaximumPendingFileChooseRequests) {
    return false;
  }

  file_chooser_completions_.push_back(
      std::make_unique<PendingFileChooser>(params, completion));
  if (file_chooser_completions_.size() == 1) {
    Send(new FrameHostMsg_RunFileChooser(routing_id_, params));
  }
  return true;
}
