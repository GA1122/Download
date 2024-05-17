void SessionService::OnGotSessionCommands(
    Handle handle,
    scoped_refptr<InternalGetCommandsRequest> request) {
  if (request->canceled())
    return;

  ScopedVector<SessionWindow> valid_windows;
  RestoreSessionFromCommands(
      request->commands, &(valid_windows.get()));
  static_cast<InternalSessionRequest*>(request.get())->
      real_callback.Run(request->handle(), &(valid_windows.get()));
}
