SessionService::Handle SessionService::GetLastSession(
    CancelableRequestConsumerBase* consumer,
    const SessionCallback& callback) {
  return ScheduleGetLastSessionCommands(
      new InternalSessionRequest(
          base::Bind(&SessionService::OnGotSessionCommands,
                     base::Unretained(this)),
          callback),
      consumer);
}
