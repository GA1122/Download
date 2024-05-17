void ConnectionToClient::OnVideoInitialized(bool successful) {
  if (!successful) {
    LOG(ERROR) << "Failed to connect video channel";
    CloseOnError();
    return;
  }

  video_connected_ = true;
  NotifyIfChannelsReady();
}
