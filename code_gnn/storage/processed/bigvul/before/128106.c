void SynchronousCompositorOutputSurface::GetMessagesToDeliver(
    ScopedVector<IPC::Message>* messages) {
  DCHECK(CalledOnValidThread());
  scoped_ptr<FrameSwapMessageQueue::SendMessageScope> send_message_scope =
      frame_swap_message_queue_->AcquireSendMessageScope();
  frame_swap_message_queue_->DrainMessages(messages);
}
