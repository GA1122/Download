void ProfileSyncService::OnConnectionStatusChange(
    sync_api::ConnectionStatus status) {
  UpdateAuthErrorState(ConnectionStatusToAuthError(status));
}
