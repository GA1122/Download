PresentationConnection* PresentationConnection::take(
    PresentationReceiver* receiver,
    const WebPresentationSessionInfo& sessionInfo) {
  DCHECK(receiver);

  PresentationConnection* connection = new PresentationConnection(
      receiver->frame(), sessionInfo.id, sessionInfo.url);
  receiver->registerConnection(connection);

  return connection;
}
