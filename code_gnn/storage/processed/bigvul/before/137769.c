void HeadlessWebContentsImpl::PendingFrameReadbackComplete(
    HeadlessWebContentsImpl::PendingFrame* pending_frame,
    const SkBitmap& bitmap,
    content::ReadbackResponse response) {
  TRACE_EVENT2(
      "headless", "HeadlessWebContentsImpl::PendingFrameReadbackComplete",
      "sequence_number", pending_frame->sequence_number, "response", response);
  if (response == content::READBACK_SUCCESS) {
    pending_frame->bitmap = base::MakeUnique<SkBitmap>(bitmap);
  } else {
    LOG(WARNING) << "Readback from surface failed with response " << response;
  }

  pending_frame->wait_for_copy_result = false;

  if (pending_frame->MaybeRunCallback()) {
    base::EraseIf(pending_frames_,
                  [pending_frame](const std::unique_ptr<PendingFrame>& frame) {
                    return frame.get() == pending_frame;
                  });
  }
}
