  InternalSessionRequest(
      const CallbackType& callback,
      const SessionService::SessionCallback& real_callback)
      : BaseSessionService::InternalGetCommandsRequest(callback),
        real_callback(real_callback) {
  }
