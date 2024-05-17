bool DrawingBuffer::PrepareTextureMailboxInternal(
    viz::TextureMailbox* out_mailbox,
    std::unique_ptr<cc::SingleReleaseCallback>* out_release_callback,
    bool force_gpu_result) {
  DCHECK(state_restorer_);
  if (destruction_in_progress_) {
    return false;
  }
  DCHECK(!is_hidden_);
  if (!contents_changed_)
    return false;

  if (gl_->GetGraphicsResetStatusKHR() != GL_NO_ERROR)
    return false;

  TRACE_EVENT0("blink,rail", "DrawingBuffer::prepareMailbox");

  ResolveIfNeeded();

  if (software_rendering_ && !force_gpu_result) {
    return FinishPrepareTextureMailboxSoftware(out_mailbox,
                                               out_release_callback);
  } else {
    return FinishPrepareTextureMailboxGpu(out_mailbox, out_release_callback);
  }
}
