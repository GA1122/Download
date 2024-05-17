bool DrawingBuffer::PrepareTextureMailbox(
    viz::TextureMailbox* out_mailbox,
    std::unique_ptr<cc::SingleReleaseCallback>* out_release_callback) {
  ScopedStateRestorer scoped_state_restorer(this);
  bool force_gpu_result = false;
  return PrepareTextureMailboxInternal(out_mailbox, out_release_callback,
                                       force_gpu_result);
}
