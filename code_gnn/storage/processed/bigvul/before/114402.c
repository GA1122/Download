void GpuCommandBufferStub::OnSetGetBuffer(
    int32 shm_id, IPC::Message* reply_message) {
  if (command_buffer_.get()) {
    command_buffer_->SetGetBuffer(shm_id);
  } else {
    DLOG(ERROR) << "no command_buffer.";
    reply_message->set_reply_error();
  }
  Send(reply_message);
}
