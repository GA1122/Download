void GpuCommandBufferStub::OnReschedule() {
  while (!delayed_echos_.empty()) {
    scoped_ptr<IPC::Message> message(delayed_echos_.front());
    delayed_echos_.pop_front();

    OnMessageReceived(*message);
  }

  channel_->OnScheduled();
}
