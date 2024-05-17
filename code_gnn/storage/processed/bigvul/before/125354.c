CallbackType CreateRelayCallback(const CallbackType& callback) {
  return base::Bind(&RelayCallback<CallbackType>::Run,
                    base::MessageLoopProxy::current(),
                    callback);
}
