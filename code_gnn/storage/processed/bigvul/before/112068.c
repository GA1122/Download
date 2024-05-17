const char* ConnectionStatusToString(ConnectionStatus status) {
  switch (status) {
    case CONNECTION_OK:
      return "CONNECTION_OK";
    case CONNECTION_AUTH_ERROR:
      return "CONNECTION_AUTH_ERROR";
    case CONNECTION_SERVER_ERROR:
      return "CONNECTION_SERVER_ERROR";
    default:
      NOTREACHED();
      return "INVALID_CONNECTION_STATUS";
  }
}
