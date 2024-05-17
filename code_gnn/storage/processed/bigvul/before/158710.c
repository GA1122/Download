error::Error GLES2DecoderImpl::HandleDescheduleUntilFinishedCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  if (!gl::GLFence::IsSupported())
    return error::kNoError;
  std::unique_ptr<gl::GLFence> fence = gl::GLFence::Create();
  deschedule_until_finished_fences_.push_back(std::move(fence));

  if (deschedule_until_finished_fences_.size() == 1)
    return error::kNoError;

  DCHECK_EQ(2u, deschedule_until_finished_fences_.size());
  if (deschedule_until_finished_fences_[0]->HasCompleted()) {
    deschedule_until_finished_fences_.erase(
        deschedule_until_finished_fences_.begin());
    return error::kNoError;
  }

  TRACE_EVENT_ASYNC_BEGIN0("cc", "GLES2DecoderImpl::DescheduleUntilFinished",
                           this);
  client_->OnDescheduleUntilFinished();
  return error::kDeferLaterCommands;
}
