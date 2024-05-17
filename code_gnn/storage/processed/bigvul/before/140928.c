PresentationConnection* PresentationConnection::take(
    PresentationController* controller,
    const WebPresentationSessionInfo& sessionInfo,
    PresentationRequest* request) {
  ASSERT(controller);
  ASSERT(request);

  PresentationConnection* connection = new PresentationConnection(
      controller->frame(), sessionInfo.id, sessionInfo.url);
  controller->registerConnection(connection);

  auto* event = PresentationConnectionAvailableEvent::create(
      EventTypeNames::connectionavailable, connection);
  TaskRunnerHelper::get(TaskType::Presentation, request->getExecutionContext())
      ->postTask(BLINK_FROM_HERE,
                 WTF::bind(&PresentationConnection::dispatchEventAsync,
                           wrapPersistent(request), wrapPersistent(event)));

  return connection;
}
