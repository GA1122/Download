error::Error GLES2DecoderPassthroughImpl::DoDescheduleUntilFinishedCHROMIUM() {
  if (!gl::GLFence::IsSupported()) {
    return error::kNoError;
  }
  deschedule_until_finished_fences_.push_back(gl::GLFence::Create());

  if (deschedule_until_finished_fences_.size() == 1) {
    return error::kNoError;
  }

  DCHECK_EQ(2u, deschedule_until_finished_fences_.size());
  if (deschedule_until_finished_fences_[0]->HasCompleted()) {
    deschedule_until_finished_fences_.erase(
        deschedule_until_finished_fences_.begin());
    return error::kNoError;
  }

  TRACE_EVENT_ASYNC_BEGIN0(
      "cc", "GLES2DecoderPassthroughImpl::DescheduleUntilFinished", this);
  client()->OnDescheduleUntilFinished();
  return error::kDeferLaterCommands;
}
