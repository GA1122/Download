void Syncer::ProcessClientCommand(sessions::SyncSession* session) {
  const ClientToServerResponse& response =
      session->status_controller()->updates_response();
  if (!response.has_client_command())
    return;
  const ClientCommand& command = response.client_command();

  if (command.has_max_commit_batch_size()) {
    session->context()->set_max_commit_batch_size(
        command.max_commit_batch_size());
  }
  if (command.has_set_sync_long_poll_interval()) {
    session->delegate()->OnReceivedLongPollIntervalUpdate(
        TimeDelta::FromSeconds(command.set_sync_long_poll_interval()));
  }
  if (command.has_set_sync_poll_interval()) {
    session->delegate()->OnReceivedShortPollIntervalUpdate(
        TimeDelta::FromSeconds(command.set_sync_poll_interval()));
  }

  if (command.has_sessions_commit_delay_seconds()) {
    session->delegate()->OnReceivedSessionsCommitDelay(
        TimeDelta::FromSeconds(command.sessions_commit_delay_seconds()));
  }
}
