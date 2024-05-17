void ConnectionToClient::CloseChannels() {
  if (video_writer_.get())
    video_writer_->Close();
  if (client_control_sender_.get())
    client_control_sender_->Close();
}
