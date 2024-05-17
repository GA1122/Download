GoogleServiceAuthError ConnectionStatusToAuthError(
    sync_api::ConnectionStatus status) {
  switch (status) {
    case sync_api::CONNECTION_OK:
      return GoogleServiceAuthError::None();
      break;
    case sync_api::CONNECTION_AUTH_ERROR:
      return GoogleServiceAuthError(
          GoogleServiceAuthError::INVALID_GAIA_CREDENTIALS);
      break;
    case sync_api::CONNECTION_SERVER_ERROR:
      return GoogleServiceAuthError(GoogleServiceAuthError::CONNECTION_FAILED);
      break;
    default:
      NOTREACHED();
      return GoogleServiceAuthError(GoogleServiceAuthError::CONNECTION_FAILED);
  }
}
